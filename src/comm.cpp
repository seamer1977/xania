/*************************************************************************/
/*  Xania (M)ulti(U)ser(D)ungeon server source code                      */
/*  (C) 1995-2000 Xania Development Team                                    */
/*  See the header to file: merc.h for original code copyrights          */
/*                                                                       */
/*  comm.c: the core of the MUD, handles initialisation and networking   */
/*                                                                       */
/*************************************************************************/

/*
 * Originally contained a ton of OS-specific stuff.
 * We only support linux these days...so we stopped pretending...
 */

#include "comm.hpp"
#include "Descriptor.hpp"
#include "challeng.h"
#include "common/Fd.hpp"
#include "common/doorman_protocol.h"
#include "interp.h"
#include "merc.h"
#include "note.h"
#include "string_utils.hpp"

#include <arpa/telnet.h>
#include <cctype>
#include <cerrno>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fcntl.h>
#include <fmt/format.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string>
#include <sys/signalfd.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

using namespace std::literals;
using namespace fmt::literals;

/* Added by Rohan - extern to player list for adding new players to it */
extern KNOWN_PLAYERS *player_list;

/* extern void identd_lookup();  ......commented out due to sock leaks */

char str_boot_time[MAX_INPUT_LENGTH];

/*
 * Global variables.
 */
Descriptor *descriptor_list; /* All open descriptors    */
Descriptor *d_next; /* Next descriptor in loop */
FILE *fpReserve; /* Reserved file handle    */
bool god; /* All new chars are gods! */
bool merc_down; /* Shutdown       */
bool wizlock; /* Game is wizlocked    */
bool newlock; /* Game is newlocked    */
time_t current_time; /* time of this pulse */
bool MOBtrigger;

Descriptor *new_descriptor(int channel);
void reap_closed_sockets();
bool read_from_descriptor(Descriptor *d, std::string_view data);
void move_active_char_from_limbo(CHAR_DATA *ch);

/*
 * Other local functions (OS-independent).
 */
bool check_parse_name(const char *name);
bool check_reconnect(Descriptor *d, bool fConn);
bool check_playing(Descriptor *d, char *name);
void nanny(Descriptor *d, const char *argument);
bool process_output(Descriptor *d, bool fPrompt);
void show_prompt(Descriptor *d, char *prompt);

/* Handle to get to doorman */
Fd doormanDesc;

/* Send a packet to doorman */
bool send_to_doorman(const Packet *p, const void *extra) {
    // TODO: do something rather than return here if there's a failure
    if (!doormanDesc.is_open())
        return false;
    if (p->nExtra > PACKET_MAX_PAYLOAD_SIZE) {
        bug("MUD tried to send a doorman packet with payload size %d > %d! Dropping!", p->nExtra,
            PACKET_MAX_PAYLOAD_SIZE);
        return false;
    }
    try {
        if (p->nExtra)
            doormanDesc.write_many(*p, gsl::span<const byte>(static_cast<const byte *>(extra), p->nExtra));
        else
            doormanDesc.write(*p);
    } catch (const std::runtime_error &re) {
        bug("%s", "Unable to write to doorman: {}"_format(re.what()).c_str());
        return false;
    }
    return true;
}

void SetEchoState(Descriptor *d, int on) {
    Packet p;
    p.type = on ? PACKET_ECHO_ON : PACKET_ECHO_OFF;
    p.channel = d->channel();
    p.nExtra = 0;
    send_to_doorman(&p, nullptr);
}

/* where we're asked nicely to quit from the outside (mudmgr or OS) */
void handle_signal_shutdown() {
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    Descriptor *d, *d_next;

    log_string("Signal shutdown received");

    /* ask everyone to save! */
    for (vch = char_list; vch != nullptr; vch = vch_next) {
        vch_next = vch->next;

        // vch->d->c check added by TM to avoid crashes when
        // someone hasn't logged in but the mud is shut down
        if (!IS_NPC(vch) && vch->desc && vch->desc->is_playing()) {
            /* Merc-2.2 MOBProgs - Faramir 31/8/1998 */
            MOBtrigger = false;
            do_save(vch, "");
            send_to_char("|RXania has been asked to shutdown by the operating system.|w\n\r", vch);
            if (vch->desc && vch->desc->has_buffered_output())
                process_output(vch->desc, false);
        }
    }
    merc_down = true;
    for (d = descriptor_list; d != nullptr; d = d_next) {
        d_next = d->next;
        close_socket(d);
    }
}

Fd init_socket(const char *file) {
    unlink(file);

    auto fd = Fd::socket(PF_UNIX, SOCK_STREAM, 0);
    sockaddr_un xaniaAddr{};
    strncpy(xaniaAddr.sun_path, file, sizeof(xaniaAddr.sun_path));
    xaniaAddr.sun_family = PF_UNIX;
    int enabled = 1;
    fd.setsockopt(SOL_SOCKET, SO_REUSEADDR, &enabled, sizeof(enabled)).bind(xaniaAddr).listen(3);

    log_string("Waiting for doorman...");
    try {
        sockaddr_un dont_care{};
        socklen_t dont_care_len{sizeof(sockaddr_in)};
        doormanDesc = fd.accept(reinterpret_cast<sockaddr *>(&dont_care), &dont_care_len);
        log_string("Doorman has connected - proceeding with boot-up");
    } catch (const std::system_error &e) {
        log_string("Unable to accept doorman...booting anyway");
    }

    return fd;
}

void doorman_lost() {
    log_string("Lost connection to doorman");
    doormanDesc.close();
    /* Now to go through and disconnect all the characters */
    for (auto *desc = descriptor_list; desc; desc = desc->next)
        close_socket(descriptor_list);
}

void handle_doorman_packet(const Packet &p, std::string_view buffer) {
    switch (p.type) {
    case PACKET_CONNECT:
        log_string("Incoming connection on channel {}."_format(p.channel));
        new_descriptor(p.channel);
        break;
    case PACKET_RECONNECT: {
        log_string("Incoming reconnection on channel {} for {}."_format(p.channel, buffer));
        auto *d = new_descriptor(p.channel);
        // Login name
        d->write(buffer);
        d->write("\n\r");
        std::string nannyable(buffer);
        nanny(d, nannyable.c_str()); // TODO one day string_viewify
        d->write("\n\r");

        // Paranoid :
        if (d->state() == DescriptorState::GetOldPassword) {
            d->state(DescriptorState::CircumventPassword);
            // log in
            nanny(d, "");
            // accept ANSIness
            d->write("\n\r");
            nanny(d, "");
        }
    } break;
    case PACKET_DISCONNECT:
        /*
         * Find the descriptor associated with the channel and close it
         */
        for (auto *d = descriptor_list; d; d = d->next) {
            if (d->channel() == p.channel) {
                if (d->character() && d->character()->level > 1)
                    save_char_obj(d->character());
                d->clear_output_buffer();
                if (d->is_playing())
                    d->state(DescriptorState::Disconnecting);
                else
                    d->state(DescriptorState::DisconnectingNp);
                close_socket(d);
                break;
            }
        }
        break;
    case PACKET_INFO: {
        /*
         * Find the descriptor associated with the channel
         */
        Descriptor *found{};
        for (auto *d = descriptor_list; d; d = d->next) {
            if (d->channel() == p.channel) {
                found = d;
                auto *data = reinterpret_cast<const InfoData *>(buffer.data());
                d->set_endpoint(data->netaddr, data->port, data->data);
                break;
            }
        }
        if (!found) {
            log_string("Unable to associate info with a descriptor ({})"_format(p.channel));
        } else {
            log_string("Info from doorman: {} is {}"_format(p.channel, found->host()));
        }
        break;
    }
    case PACKET_MESSAGE:
        /*
         * Find the descriptor associated with the channel
         */
        {
            bool ok = false;
            for (auto *d = descriptor_list; d; d = d->next) {
                if (d->channel() == p.channel) {
                    ok = true;
                    if (d->character() != nullptr)
                        d->character()->timer = 0;
                    read_from_descriptor(d, buffer);
                }
            }
            if (!ok) {
                log_string("Unable to associate message with a descriptor ({})"_format(p.channel));
            }
        }
        break;
    default: break;
    }
}

void game_loop_unix(Fd control) {
    static timeval null_time;

    signal(SIGPIPE, SIG_IGN);

    sigset_t signals;
    sigemptyset(&signals);
    sigaddset(&signals, SIGTERM);
    sigaddset(&signals, SIGINT);
    sigprocmask(SIG_BLOCK, &signals, nullptr);
    int signal_fd = signalfd(-1, &signals, SFD_NONBLOCK);
    if (signal_fd < 0) {
        perror("signal_fd");
        return;
    }

    timeval last_time{};
    gettimeofday(&last_time, nullptr);
    current_time = (time_t)last_time.tv_sec;

    /* Main loop */
    while (!merc_down) {
        fd_set in_set;
        fd_set out_set;
        fd_set exc_set;
        int maxdesc;

        /*
         * Poll all active descriptors.
         */
        FD_ZERO(&in_set);
        FD_ZERO(&out_set);
        FD_ZERO(&exc_set);
        FD_SET(control.number(), &in_set);
        FD_SET(signal_fd, &in_set);
        if (doormanDesc.is_open()) {
            FD_SET(doormanDesc.number(), &in_set);
            maxdesc = std::max(control.number(), doormanDesc.number());
        } else
            maxdesc = control.number();
        maxdesc = std::max(maxdesc, signal_fd);

        if (select(maxdesc + 1, &in_set, &out_set, &exc_set, &null_time) < 0) {
            perror("Game_loop: select: poll");
            exit(1);
        }

        if (FD_ISSET(signal_fd, &in_set)) {
            struct signalfd_siginfo info;
            if (read(signal_fd, &info, sizeof(info)) != sizeof(info)) {
                bug("Unable to read signal info - treating as term");
                info.ssi_signo = SIGTERM;
            }
            if (info.ssi_signo == SIGTERM || info.ssi_signo == SIGINT) {
                handle_signal_shutdown();
                return;
            } else {
                bug("Unexpected signal %d: ignoring", info.ssi_signo);
            }
        }

        /* If we don't have a connection from doorman, wait for one */
        if (!doormanDesc.is_open()) {
            log_string("Waiting for doorman...");

            try {
                sockaddr_un dont_care{};
                socklen_t dont_care_len{sizeof(sockaddr_in)};
                doormanDesc = control.accept(reinterpret_cast<sockaddr *>(&dont_care), &dont_care_len);
                log_string("Doorman has connected.");
                Packet pInit;
                pInit.nExtra = pInit.channel = 0;
                pInit.type = PACKET_INIT;
                send_to_doorman(&pInit, nullptr);
            } catch (const std::runtime_error &re) {
                bug("%s", "Unable to accept doorman connection: {}"_format(re.what()).c_str());
            }
        }

        /* Process doorman input, if any */
        if (doormanDesc.is_open() && FD_ISSET(doormanDesc.number(), &in_set)) {
            Packet p{};
            std::string payload;
            do {
                try {
                    p = doormanDesc.read_all<Packet>();
                    if (p.nExtra) {
                        if (p.nExtra > PACKET_MAX_PAYLOAD_SIZE) {
                            bug("Doorman sent a too big packet! %d > %d: dropping", p.nExtra, PACKET_MAX_PAYLOAD_SIZE);
                            doorman_lost();
                            break;
                        }
                        payload.resize(p.nExtra);
                        doormanDesc.read_all(gsl::span<char>(payload));
                    }
                } catch (const std::runtime_error &re) {
                    bug("%s", "Unable to read doorman packet: {}"_format(re.what()).c_str());
                    doorman_lost();
                    break;
                }
                handle_doorman_packet(p, payload);
                /* Reselect to see if there is more data */
                select(maxdesc + 1, &in_set, &out_set, &exc_set, &null_time);
            } while FD_ISSET(doormanDesc.number(), &in_set);
        }

        /*
         * De-waitstate characters and process pending input
         */
        std::unordered_set<Descriptor *> handled_input;
        for (auto *d = descriptor_list; d; d = d->next) {
            d->processing_command(false);
            auto *character = d->character();
            if (character && character->wait > 0)
                --character->wait;
            /* Waitstate the character */
            if (character && character->wait)
                continue;

            if (auto incomm = d->pop_incomm()) {
                d->processing_command(true);
                move_active_char_from_limbo(character);

                // It's possible that 'd' will be deleted as a result of any of the following operations. Be very
                // careful.
                if (d->is_paging())
                    d->show_next_page(*incomm);
                else if (d->is_playing())
                    interpret(character, incomm->c_str());
                else
                    nanny(d, incomm->c_str());
            }
        }

        // Reap any sockets that got closed as a result of input processing or nannying.
        reap_closed_sockets();

        /*
         * Autonomous game motion.
         */
        update_handler();

        /*
         * Output.
         */
        if (doormanDesc.is_open()) {
            for (auto *d = descriptor_list; d != nullptr; d = d->next) {
                if (d->processing_command() || d->has_buffered_output()) {
                    if (!process_output(d, true)) {
                        if (d->character() != nullptr && d->character()->level > 1)
                            save_char_obj(d->character());
                        d->clear_output_buffer();
                        close_socket(d);
                    }
                }
            }
            // Reap any sockets that got closed as a result of output handling.
            reap_closed_sockets();
        }

        /*
         * Synchronize to a clock.
         * Sleep( last_time + 1/PULSE_PER_SECOND - now ).
         * Careful here of signed versus unsigned arithmetic.
         */
        {
            struct timeval now_time;
            long secDelta;
            long usecDelta;

            gettimeofday(&now_time, nullptr);
            usecDelta = ((int)last_time.tv_usec) - ((int)now_time.tv_usec) + 1000000 / PULSE_PER_SECOND;
            secDelta = ((int)last_time.tv_sec) - ((int)now_time.tv_sec);
            while (usecDelta < 0) {
                usecDelta += 1000000;
                secDelta -= 1;
            }

            while (usecDelta >= 1000000) {
                usecDelta -= 1000000;
                secDelta += 1;
            }

            if (secDelta > 0 || (secDelta == 0 && usecDelta > 0)) {
                timeval stall_time;
                stall_time.tv_usec = usecDelta;
                stall_time.tv_sec = secDelta;
                if (select(0, nullptr, nullptr, nullptr, &stall_time) < 0) {
                    perror("Game_loop: select: stall");
                    exit(1);
                }
            }
        }

        gettimeofday(&last_time, nullptr);
        current_time = (time_t)last_time.tv_sec;
    }
}

Descriptor *new_descriptor(int channel) {
    auto *dnew = new Descriptor(channel);

    /*
     * Init descriptor data.
     */
    dnew->next = descriptor_list;
    descriptor_list = dnew;

    /*
     * Send the greeting.
     */
    {
        extern char *help_greeting;
        if (help_greeting[0] == '.')
            dnew->write(help_greeting + 1);
        else
            dnew->write(help_greeting);
    }

    return dnew;
}

void close_socket(Descriptor *dclose) { dclose->close(); }

void reap_closed_sockets() {
    Descriptor *dnext;
    Descriptor **dprev = &descriptor_list;
    for (auto *desc = descriptor_list; desc; desc = dnext) {
        dnext = desc->next;
        if (desc->closed()) {
            *dprev = dnext;
            // TODO: ideally no naked news/deletes. One day.
            delete desc;
        } else {
            dprev = &desc->next;
        }
    }
}

bool read_from_descriptor(Descriptor *d, std::string_view data) {
    if (d->is_input_full()) {
        snprintf(log_buf, LOG_BUF_SIZE, "%s input overflow!", d->host().c_str());
        log_string(log_buf);
        d->write_direct("\n\r*** PUT A LID ON IT!!! ***\n\r");
        d->clear_input();
        d->add_command("quit");
        return false;
    }

    d->add_command(data);
    return true;
}

/*
 * Low level output function.
 */
bool process_output(Descriptor *d, bool fPrompt) {
    extern bool merc_down;

    /*
     * Bust a prompt.
     */
    if (!merc_down && d->is_paging())
        d->write("[Hit Return to continue]\n\r");
    else if (fPrompt && !merc_down && d->is_playing()) {
        CHAR_DATA *ch;
        CHAR_DATA *victim;

        ch = d->character();

        /* battle prompt */
        if ((victim = ch->fighting) != nullptr) {
            int percent;
            char wound[100];
            char buf[MAX_STRING_LENGTH];

            if (victim->max_hit > 0)
                percent = victim->hit * 100 / victim->max_hit;
            else
                percent = -1;

            if (percent >= 100)
                snprintf(wound, sizeof(wound), "is in excellent condition.");
            else if (percent >= 90)
                snprintf(wound, sizeof(wound), "has a few scratches.");
            else if (percent >= 75)
                snprintf(wound, sizeof(wound), "has some small wounds and bruises.");
            else if (percent >= 50)
                snprintf(wound, sizeof(wound), "has quite a few wounds.");
            else if (percent >= 30)
                snprintf(wound, sizeof(wound), "has some big nasty wounds and scratches.");
            else if (percent >= 15)
                snprintf(wound, sizeof(wound), "looks pretty hurt.");
            else if (percent >= 0)
                snprintf(wound, sizeof(wound), "is in awful condition.");
            else
                snprintf(wound, sizeof(wound), "is bleeding to death.");

            snprintf(buf, sizeof(buf), "%s %s \n\r", IS_NPC(victim) ? victim->short_descr : victim->name, wound);
            buf[0] = UPPER(buf[0]);
            d->write(buf);
        }

        ch = d->person();
        if (!IS_SET(ch->comm, COMM_COMPACT))
            d->write("\n\r");

        if (IS_SET(ch->comm, COMM_PROMPT)) {
            /* get the prompt for the character in question */

            show_prompt(d, ch->pcdata->prompt);
        }
    }

    return d->flush_output();
}

/*
 * Deal with sockets that haven't logged in yet.
 */
void nanny(Descriptor *d, const char *argument) {
    Descriptor *d_old, *d_next;
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *ch;
    char *pwdnew;
    char *p;
    int iClass;
    int race;
    int i;
    int notes;
    bool fOld;
    KNOWN_PLAYERS *temp_known_player;
    /* Rohan: 2 variables needed to increase player count */
    int count;
    Descriptor *de;

    while (isspace(*argument))
        argument++;

    ch = d->character();

    switch (d->state()) {

    default:
        bug("Nanny: bad d->state() %d.", static_cast<int>(d->state()));
        close_socket(d);
        return;

    case DescriptorState::Closed:
        // Do nothing if the descriptor is already closed.
        return;

    case DescriptorState::GetName: {
        if (argument[0] == '\0') {
            close_socket(d);
            return;
        }
        // Take a copy of the character's proposed name, so we can forcibly upper-case its first letter.
        std::string char_name(argument);
        char_name[0] = toupper(char_name[0]);
        if (!check_parse_name(char_name.c_str())) {
            d->write("Illegal name, try another.\n\rName: ");
            return;
        }

        /* TM's attempt number 2 to prevent object cloning under the
              suspicion that the other wy was causing a hang on lagged entry */
        /*      for ( ch = char_list ; ch ; ch=ch->next ) {
                 if ( !IS_NPC(ch) &&
                 (!str_cmp(ch->name,argument)) &&
                 (ch->desc != nullptr) ) {
                    write_to_buffer( d, "Illegal name, try another.\n\rName: ", 0 );
                    return;
                 }
                 } */

        fOld = load_char_obj(d, char_name.c_str());
        ch = d->character();

        if (IS_SET(ch->act, PLR_DENY)) {
            snprintf(log_buf, LOG_BUF_SIZE, "Denying access to %s@%s.", char_name.c_str(), d->host().c_str());
            log_string(log_buf);
            d->write("You are denied access.\n\r");
            close_socket(d);
            return;
        }

        if (check_reconnect(d, false)) {
            fOld = true;
        } else {
            if (wizlock && !IS_IMMORTAL(ch)) {
                d->write("The game is wizlocked.  Try again later - a reboot may be imminent.\n\r");
                close_socket(d);
                return;
            }
        }

        if (fOld) {
            /* Old player */
            d->write("Password: ");
            SetEchoState(d, 0);
            d->state(DescriptorState::GetOldPassword);
            return;
        } else {
            /* New player */
            if (newlock) {
                d->write("The game is newlocked.  Try again later - a reboot may be imminent.\n\r");
                close_socket(d);
                return;
            }

            // Check for a newban on player's site. This is the one time we use the full host name.
            if (check_ban(d->raw_full_hostname().c_str(), BAN_NEWBIES)
                || check_ban(d->raw_full_hostname().c_str(), BAN_PERMIT)) {
                d->write("Your site has been banned.  Only existing players from your site may connect.\n\r");
                close_socket(d);
                return;
            }

            snprintf(buf, sizeof(buf), "Did I hear that right -  '%s' (Y/N)? ", char_name.c_str());
            d->write(buf);
            d->state(DescriptorState::ConfirmNewName);
            return;
        }
    } break;

    case DescriptorState::GetOldPassword:
        d->write("\n\r");

        // TODO crypt can return null if if fails (e.g. password is truncated).
        // for now we just pwd[0], which lets us reset passwords.
        if (ch->pcdata->pwd[0] && strcmp(crypt(argument, ch->pcdata->pwd), ch->pcdata->pwd)) {
            d->write("Our survey said <Crude buzzer noise>.\n\rWrong password.\n\r");
            close_socket(d);
            return;
        }
        // falls through
    case DescriptorState::CircumventPassword:
        if (ch->pcdata->pwd[0] == 0) {
            d->write("Oopsie! Null password!\n\r");
            d->write("Unless some IMM has been fiddling, then this is a bug!\n\r");
            d->write("Type 'password null <new password>' to fix.\n\r");
        }

        SetEchoState(d, 1);

        // This is the one time we use the full host name.
        if (check_ban(d->raw_full_hostname().c_str(), BAN_PERMIT) && (!is_set_extra(ch, EXTRA_PERMIT))) {
            d->write("Your site has been banned.  Sorry.\n\r");
            close_socket(d);
            return;
        }

        if (check_playing(d, ch->name))
            return;

        if (check_reconnect(d, true))
            return;

        snprintf(log_buf, LOG_BUF_SIZE, "%s@%s has connected.", ch->name, d->host().c_str());
        log_new(log_buf, EXTRA_WIZNET_DEBUG,
                ((IS_SET(ch->act, PLR_WIZINVIS) || IS_SET(ch->act, PLR_PROWL))) ? get_trust(ch) : 0);

        d->write("Does your terminal support ANSI colour (Y/N/Return = as saved)?");
        d->state((d->state() == DescriptorState::CircumventPassword) ? DescriptorState::ReadMotd
                                                                     : DescriptorState::GetAnsi);

        break;

        /* RT code for breaking link */

    case DescriptorState::BreakConnect:
        switch (*argument) {
        case 'y':
        case 'Y':
            for (d_old = descriptor_list; d_old != nullptr; d_old = d_next) {
                d_next = d_old->next;
                if (d_old == d || d_old->character() == nullptr)
                    continue;

                if (str_cmp(ch->name, d_old->character()->name))
                    continue;

                close_socket(d_old);
            }
            if (check_reconnect(d, true))
                return;
            d->write("Reconnect attempt failed.\n\rName: ");
            if (d->character()) {
                free_char(d->character());
                d->character(nullptr);
            }
            d->state(DescriptorState::GetName);
            break;

        case 'n':
        case 'N':
            d->write("Name: ");
            if (d->character()) {
                free_char(d->character());
                d->character(nullptr);
            }
            d->state(DescriptorState::GetName);
            break;

        default: d->write("Please type Y or N? "); break;
        }
        break;

    case DescriptorState::ConfirmNewName:
        switch (*argument) {
        case 'y':
        case 'Y':
            snprintf(buf, sizeof(buf),
                     "Welcome new character, to Xania.\n\rThink of a password for %s: ", (char *)ch->name);
            SetEchoState(d, 0);
            d->write(buf);
            d->state(DescriptorState::GetNewPassword);
            break;

        case 'n':
        case 'N':
            d->write("Ack! Amateurs! Try typing it in properly: ");
            free_char(d->character());
            d->character(nullptr);
            d->state(DescriptorState::GetName);
            break;

        default: d->write("It's quite simple - type Yes or No: "); break;
        }
        break;

    case DescriptorState::GetAnsi:
        if (argument[0] == '\0') {
            if (ch->pcdata->colour) {
                send_to_char("This is a |RC|GO|BL|rO|gU|bR|cF|YU|PL |RM|GU|BD|W!\n\r", ch);
            }
            if (IS_HERO(ch)) {
                do_help(ch, "imotd");
                d->state(DescriptorState::ReadIMotd);
            } else {
                do_help(ch, "motd");
                d->state(DescriptorState::ReadMotd);
            }
        } else {
            switch (*argument) {
            case 'y':
            case 'Y':
                ch->pcdata->colour = true;
                send_to_char("This is a |RC|GO|BL|rO|gU|bR|cF|YU|PL |RM|GU|BD|W!\n\r", ch);
                if (IS_HERO(ch)) {
                    do_help(ch, "imotd");
                    d->state(DescriptorState::ReadIMotd);
                } else {
                    do_help(ch, "motd");
                    d->state(DescriptorState::ReadMotd);
                }
                break;

            case 'n':
            case 'N':
                ch->pcdata->colour = false;
                if (IS_HERO(ch)) {
                    do_help(ch, "imotd");
                    d->state(DescriptorState::ReadIMotd);
                } else {
                    do_help(ch, "motd");
                    d->state(DescriptorState::ReadMotd);
                }
                break;

            default: d->write("Please type Yes or No press return: "); break;
            }
        }
        break;

    case DescriptorState::GetNewPassword:
        d->write("\n\r");

        if (strlen(argument) < 5) {
            d->write("Password must be at least five characters long.\n\rPassword: ");
            return;
        }

        pwdnew = crypt(argument, ch->name);
        for (p = pwdnew; *p != '\0'; p++) {
            if (*p == '~') {
                d->write("New password not acceptable, try again.\n\rPassword: ");
                return;
            }
        }

        free_string(ch->pcdata->pwd);
        ch->pcdata->pwd = str_dup(pwdnew);
        d->write("Please retype password: ");
        d->state(DescriptorState::ConfirmNewPassword);
        break;

    case DescriptorState::ConfirmNewPassword:
        d->write("\n\r");

        if (strcmp(crypt(argument, ch->pcdata->pwd), ch->pcdata->pwd)) {
            d->write("You could try typing the same thing in twice...\n\rRetype password: ");
            d->state(DescriptorState::GetNewPassword);
            return;
        }

        SetEchoState(d, 1);
        d->write("The following races are available:\n\r  ");
        for (race = 1; race_table[race].name != nullptr; race++) {
            if (!race_table[race].pc_race)
                break;
            d->write(race_table[race].name);
            d->write(" ");
        }
        d->write("\n\r");
        d->write("What is your race (help for more information)? ");
        d->state(DescriptorState::GetNewRace);
        break;

    case DescriptorState::GetNewRace:
        one_argument(argument, arg);

        if (!strcmp(arg, "help")) {
            argument = one_argument(argument, arg);
            if (argument[0] == '\0')
                do_help(ch, "race help");
            else
                do_help(ch, argument);
            d->write("What is your race (help for more information)? ");
            break;
        }

        race = race_lookup(argument);

        if (race == 0 || !race_table[race].pc_race) {
            d->write("That is not a valid race.\n\r");
            d->write("The following races are available:\n\r  ");
            for (race = 1; race_table[race].name != nullptr; race++) {
                if (!race_table[race].pc_race)
                    break;
                d->write(race_table[race].name);
                d->write(" ");
            }
            d->write("\n\r");
            d->write("What is your race? (help for more information) ");
            break;
        }

        ch->race = race;
        /* initialize stats */
        for (i = 0; i < MAX_STATS; i++)
            ch->perm_stat[i] = pc_race_table[race].stats[i];
        ch->affected_by = (int)(ch->affected_by | race_table[race].aff);
        ch->imm_flags = ch->imm_flags | race_table[race].imm;
        ch->res_flags = ch->res_flags | race_table[race].res;
        ch->vuln_flags = ch->vuln_flags | race_table[race].vuln;
        ch->form = race_table[race].form;
        ch->parts = race_table[race].parts;

        /* add skills */
        for (i = 0; i < 5; i++) {
            if (pc_race_table[race].skills[i] == nullptr)
                break;
            group_add(ch, pc_race_table[race].skills[i], false);
        }
        /* add cost */
        ch->pcdata->points = pc_race_table[race].points;
        ch->size = pc_race_table[race].size;

        d->write("What is your sex (M/F)? ");
        d->state(DescriptorState::GetNewSex);
        break;

    case DescriptorState::GetNewSex:
        switch (argument[0]) {
        case 'm':
        case 'M':
            ch->sex = SEX_MALE;
            ch->pcdata->true_sex = SEX_MALE;
            break;
        case 'f':
        case 'F':
            ch->sex = SEX_FEMALE;
            ch->pcdata->true_sex = SEX_FEMALE;
            break;
        default: d->write("That's not a sex.\n\rWhat IS your sex? "); return;
        }

        strcpy(buf, "The following classes are available: ");
        for (iClass = 0; iClass < MAX_CLASS; iClass++) {
            if (iClass > 0)
                strcat(buf, " ");
            strcat(buf, class_table[iClass].name);
        }
        strcat(buf, "\n\r");
        d->write(buf);
        d->write("What is your class (help for more information)? ");
        d->state(DescriptorState::GetNewClass);
        break;

    case DescriptorState::GetNewClass:
        one_argument(argument, arg);
        if (!strcmp(arg, "help")) {
            argument = one_argument(argument, arg);
            if (argument[0] == '\0')
                do_help(ch, "classes");
            else
                do_help(ch, argument);
            d->write("What is your class (help for more information)? ");
            break;
        }
        iClass = class_lookup(argument);
        if (iClass == -1) {
            d->write("That's not a class.\n\rWhat IS your class? ");
            return;
        }
        ch->class_num = iClass;
        snprintf(log_buf, LOG_BUF_SIZE, "%s@%s new player.", ch->name, d->host().c_str());
        log_string(log_buf);
        d->write("\n\r");
        d->write("You may be good, neutral, or evil.\n\r");
        d->write("Which alignment (G/N/E)? ");
        d->state(DescriptorState::GetAlignment);
        break;

    case DescriptorState::GetAlignment:
        switch (argument[0]) {
        case 'g':
        case 'G': ch->alignment = 750; break;
        case 'n':
        case 'N': ch->alignment = 0; break;
        case 'e':
        case 'E': ch->alignment = -750; break;
        default:
            d->write("That's not a valid alignment.\n\r");
            d->write("Which alignment (G/N/E)? ");
            return;
        }

        d->write("\n\r");

        group_add(ch, "rom basics", false);
        group_add(ch, class_table[ch->class_num].base_group, false);
        ch->pcdata->learned[gsn_recall] = 50;
        d->write("Do you wish to customize this character?\n\r");
        d->write("Customization takes time, but allows a wider range of skills and abilities.\n\r");
        d->write("Customize (Y/N)? ");
        d->state(DescriptorState::DefaultChoice);
        break;

    case DescriptorState::DefaultChoice:
        d->write("\n\r");
        switch (argument[0]) {
        case 'y':
        case 'Y':
            ch->gen_data = (GEN_DATA *)alloc_perm(sizeof(*ch->gen_data));
            ch->gen_data->points_chosen = ch->pcdata->points;
            do_help(ch, "group header");
            list_group_costs(ch);
            d->write("You already have the following skills:\n\r");
            do_skills(ch, "");
            do_help(ch, "menu choice");
            d->state(DescriptorState::GenGroups);
            break;
        case 'n':
        case 'N':
            group_add(ch, class_table[ch->class_num].default_group, true);
            d->write("\n\r");
            d->write("Does your terminal support ANSI colour (Y/N/Return = as saved)?");
            d->state(DescriptorState::GetAnsi);

            break;
        default: d->write("Please answer (Y/N)? "); return;
        }
        break;

    case DescriptorState::GenGroups:
        send_to_char("\n\r", ch);
        if (!str_cmp(argument, "done")) {
            snprintf(buf, sizeof(buf), "Creation points: %d\n\r", ch->pcdata->points);
            send_to_char(buf, ch);
            snprintf(buf, sizeof(buf), "Experience per level: %d\n\r", exp_per_level(ch, ch->gen_data->points_chosen));
            if (ch->pcdata->points < 40)
                ch->train = (40 - ch->pcdata->points + 1) / 2;
            send_to_char(buf, ch);
            d->write("\n\r");
            d->write("Does your terminal support ANSI colour (Y/N/Return = as saved)?");
            d->state(DescriptorState::GetAnsi);

            break;
        }

        if (!parse_gen_groups(ch, argument))
            send_to_char("Choices are: list,learned,premise,add,drop,info,help, and done.\n\r", ch);

        do_help(ch, "menu choice");
        break;

    case DescriptorState::ReadIMotd:
        d->write("\n\r");
        do_help(ch, "motd");
        d->state(DescriptorState::ReadMotd);
        break;

    case DescriptorState::ReadMotd:
        d->write("\n\rWelcome to Xania.  May your stay be eventful.\n\r");
        ch->next = char_list;
        char_list = ch;
        d->state(DescriptorState::Playing);
        reset_char(ch);

        /* Moog: tell doorman we logged in OK */
        {
            Packet p;
            p.type = PACKET_AUTHORIZED;
            p.channel = d->channel();
            p.nExtra = strlen(ch->name) + 1;
            send_to_doorman(&p, ch->name);
        }

        if (ch->level == 0) {

            if (ch->race != race_lookup("dragon"))
                ch->perm_stat[class_table[ch->class_num].attr_prime] += 3;

            ch->level = 1;
            ch->exp = exp_per_level(ch, ch->pcdata->points);
            ch->hit = ch->max_hit;
            ch->mana = ch->max_mana;
            ch->move = ch->max_move;
            ch->train = 3;
            ch->practice = 5;
            snprintf(buf, sizeof(buf), "the %s", title_table[ch->class_num][ch->level][ch->sex == SEX_FEMALE ? 1 : 0]);
            set_title(ch, buf);

            do_outfit(ch, "");
            obj_to_char(create_object(get_obj_index(OBJ_VNUM_MAP), 0), ch);

            ch->pcdata->learned[get_weapon_sn(ch)] = 40;

            char_to_room(ch, get_room_index(ROOM_VNUM_SCHOOL));
            send_to_char("\n\r", ch);
            do_help(ch, "NEWBIE INFO");
            send_to_char("\n\r", ch);

            /* Rohan: New player logged in, need to add name to player list */
            temp_known_player = (KNOWN_PLAYERS *)alloc_mem(sizeof(KNOWN_PLAYERS));
            temp_known_player->name = str_dup(ch->name);
            temp_known_player->next = player_list;
            player_list = temp_known_player;
            /* hack to let the newbie know about the tipwizard */
            send_to_char("|WTip: this is Xania's tip wizard! Type 'tips' to turn this on or off.|w\n\r", ch);
            /* turn on the newbie's tips */
            set_extra(ch, EXTRA_TIP_WIZARD);

        } else if (ch->in_room != nullptr) {
            char_to_room(ch, ch->in_room);
        } else if (IS_IMMORTAL(ch)) {
            char_to_room(ch, get_room_index(ROOM_VNUM_CHAT));
        } else {
            char_to_room(ch, get_room_index(ROOM_VNUM_TEMPLE));
        }

        snprintf(log_buf, LOG_BUF_SIZE, "|W### |P%s|W has entered the game.|w", ch->name);
        announce(log_buf, ch);
        act("|P$n|W has entered the game.", ch);
        do_look(ch, "auto");

        /* Rohan: code to increase the player count if needed - it was only
           updated if a player did count */
        count = 0;
        for (de = descriptor_list; de != nullptr; de = de->next)
            if (de->is_playing())
                count++;
        max_on = UMAX(count, max_on);

        if (ch->gold > 250000 && !IS_IMMORTAL(ch)) {
            snprintf(buf, sizeof(buf), "You are taxed %ld gold to pay for the Mayor's bar.\n\r",
                     (ch->gold - 250000) / 2);
            send_to_char(buf, ch);
            ch->gold -= (ch->gold - 250000) / 2;
        }

        if (ch->pet != nullptr) {
            char_to_room(ch->pet, ch->in_room);
            act("|P$n|W has entered the game.", ch->pet);
        }

        /* check notes */
        notes = note_count(ch);

        if (notes > 0) {
            snprintf(buf, sizeof(buf), "\n\rYou have %d new note%s waiting.\n\r", notes, (notes == 1) ? "" : "s");
            send_to_char(buf, ch);
        }
        break;
    }
}

/*
 * Parse a name for acceptability.
 */
bool check_parse_name(const char *name) {
    /*
     * Reserved words.
     */
    if (is_name(name, "all auto immortal self someone something the you"))
        return false;

    if (!str_cmp(capitalize(name), "DEATH"))
        return true;

    /*    if (str_cmp(capitalize(name),"DEATH") && (!str_prefix("death",name)
        || !str_suffix("Death",name)))
       return false;*/

    /*
     * Length restrictions.
     */

    if (strlen(name) < 2)
        return false;

    if (strlen(name) > 12)
        return false;

    /*
     * Alphanumerics only.
     * Lock out IllIll twits.
     */
    {
        bool fIll;

        fIll = true;
        for (const char *pc = name; *pc != '\0'; pc++) {
            if (!isalpha(*pc))
                return false;
            if (tolower(*pc) != 'i' && tolower(*pc) != 'l')
                fIll = false;
        }

        if (fIll)
            return false;
    }

    /*
     * Prevent players from naming themselves after mobs.
     */
    {
        extern MOB_INDEX_DATA *mob_index_hash[MAX_KEY_HASH];
        MOB_INDEX_DATA *pMobIndex;
        int iHash;

        for (iHash = 0; iHash < MAX_KEY_HASH; iHash++) {
            for (pMobIndex = mob_index_hash[iHash]; pMobIndex != nullptr; pMobIndex = pMobIndex->next) {
                if (is_name(name, pMobIndex->player_name))
                    return false;
            }
        }
    }

    return true;
}

/*
 * Look for link-dead player to reconnect.
 */
bool check_reconnect(Descriptor *d, bool fConn) {
    CHAR_DATA *ch;

    for (ch = char_list; ch != nullptr; ch = ch->next) {
        if (!IS_NPC(ch) && (!fConn || ch->desc == nullptr) && !str_cmp(d->character()->name, ch->name)) {
            if (fConn == false) {
                free_string(d->character()->pcdata->pwd);
                d->character()->pcdata->pwd = str_dup(ch->pcdata->pwd);
            } else {
                free_char(d->character());
                d->character(ch);
                ch->desc = d;
                ch->timer = 0;
                send_to_char("Reconnecting.\n\r", ch);
                act("$n has reconnected.", ch);
                snprintf(log_buf, LOG_BUF_SIZE, "%s@%s reconnected.", ch->name, d->host().c_str());
                log_new(log_buf, EXTRA_WIZNET_DEBUG,
                        ((IS_SET(ch->act, PLR_WIZINVIS) || IS_SET(ch->act, PLR_PROWL))) ? get_trust(ch) : 0);
                d->state(DescriptorState::Playing);
            }
            return true;
        }
    }

    return false;
}

/*
 * Check if already playing.
 */
bool check_playing(Descriptor *d, char *name) {
    Descriptor *dold;

    for (dold = descriptor_list; dold; dold = dold->next) {
        if (dold != d && dold->character() != nullptr && dold->state() != DescriptorState::GetName
            && dold->state() != DescriptorState::GetOldPassword && !str_cmp(name, dold->person()->name)) {
            d->write("That character is already playing.\n\r");
            d->write("Do you wish to connect anyway (Y/N)?");
            d->state(DescriptorState::BreakConnect);
            return true;
        }
    }

    return false;
}

// Write to one char.
void send_to_char(std::string_view txt, const CHAR_DATA *ch) {
    if (txt.empty() || ch->desc == nullptr || !ch->desc->person())
        return;

    ch->desc->write(colourise_mud_string(ch->desc->person()->pcdata->colour, txt));
}

/*
 * Send a page to one char.
 */
void page_to_char(const char *txt, CHAR_DATA *ch) {
    if (txt == nullptr || ch->desc == nullptr)
        return;

    ch->desc->page_to(txt);
}

void act(const char *format, CHAR_DATA *ch, Act1Arg arg1, Act2Arg arg2, To type) {
    act(format, ch, arg1, arg2, type, POS_RESTING);
}

namespace {

std::string_view pronouns(std::string_view format, const CHAR_DATA *ch, const std::array<std::string_view, 3> &text) {
    if (!ch) {
        bug("%s", "Act: null ch in pronouns with format '{}'"_format(format).c_str());
        return text[0];
    }
    switch (ch->sex) {
    case 0: return text[0];
    case 1: return text[1];
    case 2: return text[2];
    }
    bug("%s", "Act: bad sex {} in pronouns with format '{}'"_format(ch->sex, format).c_str());
    return text[0];
}

std::string_view he_she(std::string_view format, const CHAR_DATA *ch) {
    return pronouns(format, ch, {"it"sv, "he"sv, "she"sv});
}
std::string_view him_her(std::string_view format, const CHAR_DATA *ch) {
    return pronouns(format, ch, {"it"sv, "him"sv, "her"sv});
}
std::string_view his_her(std::string_view format, const CHAR_DATA *ch) {
    return pronouns(format, ch, {"its"sv, "his"sv, "her"sv});
}

std::string format_act(std::string_view format, const CHAR_DATA *ch, Act1Arg arg1, Act2Arg arg2, const CHAR_DATA *to,
                       const CHAR_DATA *vch) {
    std::string buf;

    bool prev_was_dollar = false;
    for (auto c : format) {
        if (!std::exchange(prev_was_dollar, false)) {
            if (c != '$') {
                prev_was_dollar = false;
                buf.push_back(c);
            } else {
                prev_was_dollar = true;
            }
            continue;
        }

        if (std::holds_alternative<nullptr_t>(arg2) && c >= 'A' && c <= 'Z') {
            bug("%s", "Act: missing arg2 for code {} in format '{}'"_format(c, format).c_str());
            continue;
        }

        switch (c) {
        default:
            bug("%s", "Act: bad code {} in format '{}'"_format(c, format).c_str());
            break;
            /* Thx alex for 't' idea */
        case 't':
            if (auto arg1_as_string_ptr = std::get_if<std::string_view>(&arg1)) {
                buf += *arg1_as_string_ptr;
            } else {
                bug("%s", "$t passed but arg1 was not a string in '{}'"_format(format).c_str());
            }
            break;
        case 'T':
            if (auto arg2_as_string_ptr = std::get_if<std::string_view>(&arg2)) {
                buf += *arg2_as_string_ptr;
            } else {
                bug("%s", "$T passed but arg2 was not a string in '{}'"_format(format).c_str());
            }
            break;
        case 'n': buf += pers(ch, to); break;
        case 'N': buf += pers(vch, to); break;
        case 'e': buf += he_she(format, ch); break;
        case 'E': buf += he_she(format, vch); break;
        case 'm': buf += him_her(format, ch); break;
        case 'M': buf += him_her(format, vch); break;
        case 's': buf += his_her(format, ch); break;
        case 'S': buf += his_her(format, vch); break;

        case 'p':
            if (auto arg1_as_obj_ptr = std::get_if<const OBJ_DATA *>(&arg1)) {
                auto &obj1 = *arg1_as_obj_ptr;
                buf += can_see_obj(to, obj1) ? obj1->short_descr : "something";
            } else {
                bug("%s", "$p passed but arg1 was not an object in '{}'"_format(format).c_str());
                buf += "something";
            }
            break;

        case 'P':
            if (auto arg2_as_obj_ptr = std::get_if<const OBJ_DATA *>(&arg2)) {
                auto &obj2 = *arg2_as_obj_ptr;
                buf += can_see_obj(to, obj2) ? obj2->short_descr : "something";
            } else {
                bug("%s", "$p passed but arg2 was not an object in '{}'"_format(format).c_str());
                buf += "something";
            }
            break;

        case 'd':
            if (auto arg2_as_string_ptr = std::get_if<std::string_view>(&arg2);
                arg2_as_string_ptr != nullptr && (*arg2_as_string_ptr)[0] != '\0') {
                std::string arg2_as_string(*arg2_as_string_ptr); // TODO: once one_argument is not so sucky, change this
                char fname[MAX_INPUT_LENGTH];
                one_argument(arg2_as_string.c_str(), fname);
                buf += fname;
            } else {
                buf += "door";
            }
            break;
        }
    }

    return upper_first_character(buf) + "\n\r";
}

bool act_to_person(const CHAR_DATA *person, int min_pos) {
    // Ignore folks with no descriptor, or below minimum position.
    return person->desc != nullptr && person->position >= min_pos;
}

std::vector<const CHAR_DATA *> folks_in_room(const ROOM_INDEX_DATA *room, const CHAR_DATA *ch, const CHAR_DATA *vch,
                                             const To &type, int min_pos) {
    std::vector<const CHAR_DATA *> result;
    for (auto *person = room->people; person; person = person->next_in_room) {
        if (!act_to_person(person, min_pos))
            continue;
        // Never consider the character themselves (they're handled explicitly elsewhere).
        if (person == ch)
            continue;
        // Ignore the victim if necessary.
        if (type == To::NotVict && person == vch)
            continue;
        result.emplace_back(person);
    }
    return result;
}

std::vector<const CHAR_DATA *> collect_folks(const CHAR_DATA *ch, const CHAR_DATA *vch, Act2Arg arg2, To type,
                                             int min_pos) {
    const ROOM_INDEX_DATA *room{};

    switch (type) {
    case To::Char:
        if (act_to_person(ch, min_pos))
            return {ch};
        else
            return {};

    case To::Vict:
        if (vch == nullptr) {
            bug("Act: null or incorrect type of vch");
            return {};
        }
        if (vch->in_room == nullptr || ch == vch || !act_to_person(vch, min_pos))
            return {};

        return {vch};

    case To::GivenRoom:
        if (auto arg2_as_room_ptr = std::get_if<const ROOM_INDEX_DATA *>(&arg2)) {
            room = *arg2_as_room_ptr;
        } else {
            bug("Act: null room with To::GivenRoom.");
            return {};
        }
        break;

    case To::Room:
    case To::NotVict: room = ch->in_room; break;
    }

    auto result = folks_in_room(room, ch, vch, type, min_pos);

    // If we're sending messages to the challenge room...
    if (room->vnum == CHAL_ROOM) {
        // also include all the folks in the viewing gallery with the appropriate position. We assume the victim
        // is not somehow in the viewing gallery.
        auto viewing = folks_in_room(get_room_index(CHAL_VIEWING_GALLERY), ch, vch, type, min_pos);
        result.insert(result.end(), viewing.begin(), viewing.end());
    }

    return result;
}

}

void act(const char *format, CHAR_DATA *ch, Act1Arg arg1, Act2Arg arg2, To type, int min_pos) {
    // Socials and puff use null here as a "don't do this". If you're tempted to turn format into a string_view, beware,
    // that causes a segfault.
    if (format == nullptr || format[0] == 0 || !ch || !ch->in_room)
        return;

    const CHAR_DATA *vch = std::get_if<const CHAR_DATA *>(&arg2) ? *std::get_if<const CHAR_DATA *>(&arg2) : nullptr;

    for (auto *to : collect_folks(ch, vch, arg2, type, min_pos)) {
        auto formatted = format_act(format, ch, arg1, arg2, to, vch);
        send_to_char(formatted, to);
        /* Merc-2.2 MOBProgs - Faramir 31/8/1998 */
        if (MOBtrigger) {
            auto arg1_as_obj_ptr = std::get_if<const OBJ_DATA *>(&arg1);
            // TODO: heinous const_cast here. Safe, but annoying and worth unpicking deeper down.
            mprog_act_trigger(formatted.c_str(), const_cast<CHAR_DATA *>(to), ch,
                              arg1_as_obj_ptr ? *arg1_as_obj_ptr : nullptr, vch);
        }
    }
    MOBtrigger = true;
}

void show_prompt(Descriptor *d, char *prompt) {
    char buf[256]; /* this is actually sent to the ch */
    char buf2[64];
    CHAR_DATA *ch;
    CHAR_DATA *ch_prefix = nullptr; /* Needed for prefix in prompt with switched MOB */
    char *point;
    char *str;
    const char *i;
    int bufspace = 255; /* Counter of space left in buf[] */

    ch = d->character();
    /*
     * Discard null and zero-length prompts.
     */

    if (!IS_NPC(ch) && (ch->pcdata->colour))
        send_to_char("|p", ch);

    if (IS_NPC(ch)) {
        if (ch->desc->original())
            ch_prefix = ch->desc->original();
    } else
        ch_prefix = ch;

    if (prompt == nullptr || prompt[0] == '\0') {
        snprintf(buf, sizeof(buf), "<%d/%dhp %d/%dm %dmv> |w", ch->hit, ch->max_hit, ch->mana, ch->max_mana, ch->move);
        *(buf + strlen(buf)) = '\0';
        send_to_char(buf, ch);
        return;
    }

    point = buf;
    buf[0] = '\0';
    str = prompt; /* zero-terminated raw prompt macro */
    while ((*str != '\0') && (bufspace >= 0)) {
        float tmp, loop;
        char bar[256];

        if (*str != '%') {
            if (bufspace-- > 0) {
                *point++ = *str;
                *point = '\0';
            }
            str++;
            continue;
        }
        ++str;

        switch (*str) {
        default: i = "|W ?? |p"; break;

        case 'B':
            buf2[0] = '\0';
            bar[0] = '\0';
            strcpy(bar, "|g||");

            tmp = (float)ch->hit / (float)(ch->max_hit / 10.0);
            for (loop = 0.0; loop < 10.0; loop++) {
                if (loop > tmp) {
                    bar[2] = ' ';
                    bar[3] = '\0';
                }
                if (loop <= 10.0)
                    bar[1] = 'g';
                if (loop < 6.0)
                    bar[1] = 'y';
                if (loop < 3.0)
                    bar[1] = 'r';
                i = buf2;
                strcat(buf2, bar);
            }
            strcat(buf2, "|p");

            break;
        case 'h':
            i = buf2;
            snprintf(buf2, sizeof(buf2), "%d", ch->hit);
            break;
        case 'H':
            i = buf2;
            snprintf(buf2, sizeof(buf2), "%d", ch->max_hit);
            break;
        case 'm':
            i = buf2;
            snprintf(buf2, sizeof(buf2), "%d", ch->mana);
            break;
        case 'M':
            i = buf2;
            snprintf(buf2, sizeof(buf2), "%d", ch->max_mana);
            break;
        case 'g':
            i = buf2;
            snprintf(buf2, sizeof(buf2), "%ld", ch->gold);
            break;
        case 'x':
            i = buf2;
            snprintf(buf2, sizeof(buf2), "%ld",
                     (long int)(((long int)(ch->level + 1) * (long int)(exp_per_level(ch, ch->pcdata->points))
                                 - (long int)(ch->exp))));
            break;
        case 'v':
            i = buf2;
            snprintf(buf2, sizeof(buf2), "%d", ch->move);
            break;
        case 'V':
            i = buf2;
            snprintf(buf2, sizeof(buf2), "%d", ch->max_move);
            break;
        case 'a':
            i = buf2;
            if (get_trust(ch) >= 10) {
                snprintf(buf2, sizeof(buf2), "%d", ch->alignment);
            } else {
                snprintf(buf2, sizeof(buf2), "??");
            }
            break;
        case 'X':
            i = buf2;
            snprintf(buf2, sizeof(buf2), "%ld", ch->exp);
            break;
            /* PCFN 20-05-97 prefix */
        case 'p':
            if (ch_prefix) {
                i = ch_prefix->pcdata->prefix;
            } else {
                i = "\0";
            }
            break;
            /* end */
        case 'r':
            if (IS_IMMORTAL(ch)) {
                i = ch->in_room->name;
            } else {
                i = "|W ?? |p";
            }

            break;
        case 'W':
            if IS_IMMORTAL (ch) {
                i = (IS_SET(ch->act, PLR_WIZINVIS)) ? "|R*W*|p" : "-w-";
            } else {
                i = "|W ?? |p";
            }
            break;
            /* Can be changed easily for tribes/clans etc. */
        case 'P':
            if (IS_IMMORTAL(ch)) {
                i = (IS_SET(ch->act, PLR_PROWL)) ? "|R*P*|p" : "-p-";
            } else {
                i = "|W ?? |p";
            }
            break;
        case 'R':
            if IS_IMMORTAL (ch) {
                i = buf2;
                snprintf(buf2, sizeof(buf2), "%d", ch->in_room->vnum);
            } else {
                i = "|W ?? |p";
            }
            break;

        case 'z':
            if IS_IMMORTAL (ch) {
                i = ch->in_room->area->areaname; // print the area name short form
            } else {
                i = "|W ?? |p";
            }
            break;

        case 'n': i = "\n\r"; break;

        case 't': {
            time_t ch_timet;

            ch_timet = time(0);

            if (ch_prefix->pcdata->houroffset || ch_prefix->pcdata->minoffset) {
                struct tm *ch_time;

                ch_time = gmtime(&ch_timet);
                ch_time->tm_min += ch_prefix->pcdata->minoffset;
                ch_time->tm_hour += ch_prefix->pcdata->houroffset;

                ch_time->tm_hour -= (ch_time->tm_min / 60);
                ch_time->tm_min = (ch_time->tm_min % 60);
                if (ch_time->tm_min < 0) {
                    ch_time->tm_min += 60;
                    ch_time->tm_hour -= 1;
                }
                ch_time->tm_hour = (ch_time->tm_hour % 24);
                if (ch_time->tm_hour < 0)
                    ch_time->tm_hour += 24;

                strftime(buf2, 63, "%H:%M:%S", ch_time);
            } else
                strftime(buf2, 63, "%H:%M:%S", localtime(&ch_timet));
            i = buf2;
        } break;
        }

        ++str;
        strncpy(point, i, bufspace - 1);
        if ((int)strlen(i) > (bufspace - 1))
            point[bufspace - 1] = '\0';
        point += strlen(i);
        if ((bufspace -= strlen(i)) <= 0)
            break;

        /*      while ( ( *point = *i ) != '\0' )
                 ++point, ++i;*/
    }

    if (bufspace > 0) {
        strncpy(point, "|w", bufspace - 1);
        point[bufspace - 1] = '\0';
    }

    send_to_char(buf, ch);
}
