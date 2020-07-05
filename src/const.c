/*************************************************************************/
/*  Xania (M)ulti(U)ser(D)ungeon server source code                      */
/*  (C) 1995-2000 Xania Development Team                                    */
/*  See the header to file: merc.h for original code copyrights          */
/*                                                                       */
/*  const.c: many large tables of constant data structures               */
/*                                                                       */
/*************************************************************************/


#if defined(macintosh)
#include <types.h>
#else
#if defined(riscos)
#include "sys/types.h"
#else
#include <sys/types.h>
#endif
#endif
#include <stdio.h>
#include <time.h>
#include "merc.h"
#include "magic.h"

/* attack table  -- not very organized :( */


const 	struct attack_type	attack_table	[]	=
{
    { 	"none",		"hit",		-1		},  /*  0 */
    {	"slice",	"slice", 	DAM_SLASH	},	
    {   "stab",		"stab",		DAM_PIERCE	},
    {	"slash",	"slash",	DAM_SLASH	},
    {	"whip",		"whip",		DAM_SLASH	},
    {   "claw",		"claw",		DAM_SLASH	},  /*  5 */
    {	"blast",	"blast",	DAM_BASH	},
    {   "pound",	"pound",	DAM_BASH	},
    {	"crush",	"crush",	DAM_BASH	},
    {   "grep",		"grep",		DAM_SLASH	},
    {	"bite",		"bite",		DAM_PIERCE	},  /* 10 */
    {   "pierce",	"pierce",	DAM_PIERCE	},
    {   "suction",	"suction",	DAM_BASH	},
    {	"beating",	"beating",	DAM_BASH	},
    {   "digestion",	"|Gdigestion|w",	DAM_ACID	},
    {	"charge",	"charge",	DAM_BASH	},  /* 15 */
    { 	"slap",		"slap",		DAM_BASH	},
    {	"punch",	"punch",	DAM_BASH	},
    {	"wrath",	"wrath",	DAM_ENERGY	},
    {	"magic",	"magic",	DAM_ENERGY	},
    {   "divine",	"|Wdivine power|w",	DAM_HOLY	},  /* 20 */
    {	"cleave",	"cleave",	DAM_SLASH	},
    {	"scratch",	"scratch",	DAM_PIERCE	},
    {   "peck",		"peck",		DAM_PIERCE	},
    {   "peckb",	"peck",		DAM_BASH	},
    {   "chop",		"chop",		DAM_SLASH	},  /* 25 */
    {   "sting",	"sting",	DAM_PIERCE	},
    {   "smash",	 "smash",	DAM_BASH	},
    {   "shbite",	"|Cshocking bite|w",DAM_LIGHTNING	},
    {	"flbite",	"|Rflaming bite|w", DAM_FIRE	},
    {	"frbite",	"|Wfreezing bite|w", DAM_COLD	},  /* 30 */
    {	"acbite",	"|Yacidic bite|w", 	DAM_ACID	},
    {	"chomp",	"chomp",	DAM_PIERCE	},
    {  	"drain",	"life drain",	DAM_NEGATIVE	},
    {   "thrust",	"thrust",	DAM_PIERCE	},
    {   "slime",	"slime",	DAM_ACID	},
    {	"shock",	"shock",	DAM_LIGHTNING	},
    {   "thwack",	"thwack",	DAM_BASH	},
    {   "flame",	"flame",	DAM_FIRE	},
    {   "chill",	"chill",	DAM_COLD	},
	// Newly added to allow levels to load in without warning (TM)
	{	"maul",		"maul",		DAM_SLASH	},
	{	"hit",		"hit",		DAM_BASH	},
    {   NULL,		NULL,		0		}
};

/* race table */
/* when adding a new PC race ensure that it appears towards the top
   of this list! */
const    struct   race_type   race_table  []    =
{
   /*
       {
      name,    pc_race?,
      act bits,   aff_by bits,   off bits,
      imm,     res,     vuln,
      form,    parts
       },
   */
   {
      "unique",      FALSE, 0, 0, 0, 0, 0, 0, G, A|D|F    }
   ,

   {
      "human",    TRUE,
      0,    0,       0,
      0,       0,    0,
      A|H|M|V, A|B|C|D|E|F|G|H|I|J|K
   }
   ,

   {
      "hobbit",               TRUE,
      0,              0,              0,
      0,              0,              0,
      A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K
   }
   ,

   {
      "minotaur",              TRUE,
      0,              0,              0,
      0,              0,              0,
      A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K|W
   }
   ,

   {
      "elf",         TRUE,
      0,    AFF_INFRARED,  0,
      0,    RES_CHARM,  VULN_IRON,
      A|H|M|V, A|B|C|D|E|F|G|H|I|J|K
   }
   ,

   {
      "half-elf",               TRUE,
      0,              AFF_INFRARED,   0,
      0,              0,              VULN_IRON,
      A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K
   }
   ,


   {
      "dragon",               TRUE,
      0,              0,              0,
      0,              0,              0,
      A|H|M|V,        A|C|D|E|F|H|J|K|P|U|V|X
   }
   ,

   {
      "eagle",                TRUE,
      0,              AFF_FLYING,     0,
      0,              0,              0,
      A|H|M|V,        A|C|D|E|F|H|J|P|K|U
   }
   ,

   {
      "orc",             TRUE,
      0,              AFF_INFRARED,   0,
      0,              0,              0,
      A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K
   }
   ,

   {
      "dwarf",    TRUE,
      0,    AFF_INFRARED,  0,
      0,    RES_POISON|RES_DISEASE, VULN_DROWNING,
      A|H|M|V, A|B|C|D|E|F|G|H|I|J|K
   }
   ,

   {
      "wolf",                 TRUE,
      0,              0,              0,
      0,              0,              0,
      A|H|M|V,        A|C|D|E|F|H|J|K|Q|U|V
   }
   ,
   /* commented out for the time being --Fara
      {
      "wraith",               TRUE,
      0,              AFF_INFRARED,              0,
      IMM_COLD,              0,              VULN_FIRE,
      A|H|M|V|cc,        A|C|D|E|F|G|H|I|J|K|L|U
   }
   ,*/

   {
      "giant",    FALSE,
      0,    0,    0,
      0,    RES_FIRE|RES_COLD,   VULN_MENTAL|VULN_LIGHTNING,
      A|H|M|V, A|B|C|D|E|F|G|H|J|K
   }
   ,

   {
      "bat",         FALSE,
      0,    AFF_FLYING|AFF_DARK_VISION,   OFF_DODGE|OFF_FAST,
      0,    0,    VULN_LIGHT,
      A|G|W,      A|C|D|E|F|H|J|K|P
   }
   ,

   {
      "bear",        FALSE,
      0,    0,    OFF_CRUSH|OFF_DISARM|OFF_BERSERK,
      0,    RES_BASH|RES_COLD,   0,
      A|G|V,      A|B|C|D|E|F|H|J|K|U|V
   }
   ,

   {
      "cat",         FALSE,
      0,    AFF_DARK_VISION,  OFF_FAST|OFF_DODGE,
      0,    0,    0,
      A|G|V,      A|C|D|E|F|H|J|K|Q|U|V
   }
   ,

   {
      "centipede",      FALSE,
      0,    AFF_DARK_VISION,  0,
      0,    RES_PIERCE|RES_COLD, VULN_BASH,
      O|Y /* insect, snake  */, A|F|Q|X
   }
   ,

   {
      "dog",         FALSE,
      0,    0,    OFF_FAST,
      0,    0,    0,
      A|G|V,      A|C|D|E|F|H|J|K|U|V
   }
   ,

   {
      "doll",        FALSE,
      0,    0,    0,
      IMM_MAGIC,  RES_BASH|RES_LIGHT,
      VULN_SLASH|VULN_FIRE|VULN_ACID|VULN_LIGHTNING|VULN_ENERGY,
      E|J|M|cc,   A|B|C|G|H|K
   }
   ,

   {
      "fido",        FALSE,
      0,    0,    OFF_DODGE|ASSIST_RACE,
      0,    0,       VULN_MAGIC,
      B|G|V,      A|C|D|E|F|H|J|K|Q|V
   }
   ,

   {
      "fox",         FALSE,
      0,    AFF_DARK_VISION,  OFF_FAST|OFF_DODGE,
      0,    0,    0,
      A|G|V,      A|C|D|E|F|H|J|K|Q|U|V
   }
   ,

   {
      "goblin",      FALSE,
      0,    AFF_INFRARED,  0,
      0,    RES_DISEASE,   VULN_MAGIC,
      A|H|M|V, A|B|C|D|E|F|G|H|I|J|K
   }
   ,

   {
      "hobgoblin",      FALSE,
      0,    AFF_INFRARED,  0,
      0,    RES_DISEASE|RES_POISON, 0,
      A|H|M|V, A|B|C|D|E|F|G|H|I|J|K|Q   /* includes a tail */
     
   }
   ,

   {
      "kobold",      FALSE,
      0,    AFF_INFRARED,  0,
      0,    RES_POISON, VULN_MAGIC,
      A|B|H|M|V,  A|B|C|D|E|F|G|H|I|J|K|Q
   }
   ,

   {
      "lizard",      FALSE,
      0,    0,    0,
      0,    RES_POISON, VULN_COLD,
      A|G|X|cc,   A|C|D|E|F|H|K|Q|V
   }
   ,

   {
      "modron",      FALSE,
      0,    AFF_INFRARED,     ASSIST_RACE|ASSIST_ALIGN,
      IMM_CHARM|IMM_DISEASE|IMM_MENTAL|IMM_HOLY|IMM_NEGATIVE,
      RES_FIRE|RES_COLD|RES_ACID,   0,
      H,    A|B|C|G|H|J|K
   }
   ,

   {
      "pig",         FALSE,
      0,    0,    0,
      0,    0,    0,
      A|G|V,      A|C|D|E|F|H|J|K
   }
   ,

   {
      "rabbit",      FALSE,
      0,    0,    OFF_DODGE|OFF_FAST,
      0,    0,    0,
      A|G|V,      A|C|D|E|F|H|J|K
   }
   ,

   {
      "school monster", FALSE,
      ACT_NOALIGN,      0,    0,
      IMM_CHARM|IMM_SUMMON,   0,    VULN_MAGIC,
      A|M|V,      A|B|C|D|E|F|H|J|K|Q|U
   }
   ,

   {
      "snake",    FALSE,
      0,    0,    0,
      0,    RES_POISON, VULN_COLD,
      A|G|R|X|Y|cc,  A|D|E|F|K|L|Q|V|X
   }
   ,

   {
      "song bird",      FALSE,
      0,    AFF_FLYING,    OFF_FAST|OFF_DODGE,
      0,    0,    0,
      A|G|W,      A|C|D|E|F|H|K|P
   }
   ,

   {
      "troll",    FALSE,
      0,    AFF_REGENERATION|AFF_INFRARED|AFF_DETECT_HIDDEN,
      OFF_BERSERK,
      0, RES_CHARM|RES_BASH,  VULN_FIRE|VULN_ACID,
      B|M|V,      A|B|C|D|E|F|G|H|I|J|K|U|V
   }
   ,

   {
      "water fowl",     FALSE,
      0,    AFF_SWIM|AFF_FLYING, 0,
      0,    RES_DROWNING,     0,
      A|G|W,      A|C|D|E|F|H|K|P|Q
   }
   ,

   {
      "wyvern",      FALSE,
      0,    AFF_FLYING|AFF_DETECT_INVIS|AFF_DETECT_HIDDEN,
      OFF_BASH|OFF_FAST|OFF_DODGE,
      IMM_POISON, 0, VULN_LIGHT,
      B|Z|cc,     A|C|D|E|F|H|J|K|P|Q|V|X
   }
   ,

   {
      NULL, 0, 0, 0, 0, 0, 0
   }
};

const struct   pc_race_type   pc_race_table  [] =
{
   {
      "null race", "", 0, {
         100, 100, 100, 100       }
      ,
      {
         ""       }
      , {
         13, 13, 13, 13, 13       }
      , {
         18, 18, 18, 18, 18       }
      , 0    }
   ,

   /*
       {
      "race name",   short name,    points,  { class multipliers },
      { bonus skills },
      { base stats },      { max stats },    size
       },
   */

   {
      "human", "Human", 0, {
         100, 100, 100, 100       }
      ,
      {
	""       }
      ,
      { 14, 14, 14, 14, 14       }
      ,  {
	18, 18, 18, 18, 18       }
      ,  SIZE_MEDIUM
   }
   ,

   {
      "hobbit",        "Hobbt",        20,      {
         100, 120, 100, 120       }
      ,
      {
         "backstab", "pick lock"       }
      ,
      {
         13, 15, 16, 16, 14       }
      , {
         17, 19, 21, 20, 17       }
      , SIZE_SMALL
   }
   ,

   {
      "minotaur",        "Mintr",    15,      {
         140, 100, 150, 100       }
      ,
      {
         "headbutt"       }
      ,
      {
         16, 14, 15, 11, 16       }
      , {
         21, 18, 19, 15, 20       }
      , SIZE_LARGE
   }
   ,

   {
      "elf",      " Elf ", 5, {
         100, 125,  100, 150       }
      ,
      {
         "sneak", "hide"       }
      ,
      {
         11, 16, 14, 16, 11       }
      ,  {
         16, 21, 18, 21, 15       }
      , SIZE_MEDIUM
   }
   ,

   {
      "half-elf",     "H/Elf",       15,      {
         125, 100, 100, 125       }
      ,
      {
         "dodge"       }
      ,
      {
         14, 15, 14, 15, 14      }
      ,  {
         18, 20, 18, 19, 18      }
      , SIZE_MEDIUM
   }
   ,

   {
      "dragon",       "Dragn",       40,      {
         100, 100, 100, 100       }
      ,
      {
         "draconian", "transportation", "bash"       }
      ,
      {
         16, 16, 16, 16, 16       }
      , {
         21, 20, 20, 13, 21       }
      , SIZE_LARGE
   }
   ,


   {
      "eagle",        "Eagle",       10,       {
         110, 100, 100, 150       }
      ,
      {
         "fly", "lore"       }
      ,
      {
         13, 15, 15, 13, 14       }
      , {
         17, 20, 20, 17, 18       }
      , SIZE_SMALL
   }
   ,

   {
      "orc",          " Orc ",       10,       {
         100, 100, 120, 100       }
      ,
      {
         "fast healing", "berserk"       }
      ,
      {
         16, 12, 16, 12, 16       }
      , {
         20, 16, 20, 16, 20       }
      , SIZE_LARGE
   }
   ,


   {
      "dwarf", "Dwarf", 5, {
         150, 100, 120, 100       }
      ,
      {
         "haggle", "peek"       }
      ,
      {
         16, 12, 16, 10, 16       }
      ,  {
         20, 16, 20, 14, 21       }
      , SIZE_MEDIUM
   }
   ,


   {
      "wolf",             "Wolf ",   20,      {
         150, 150, 120, 100       }
      ,
      {
         "berserk", "second attack"       }
      ,
      {
         16, 11, 15, 16, 15       }
      , {
         21, 14, 19, 21, 19       }
      , SIZE_MEDIUM
   }
   ,
   /* commented out for the time being --Fara
   {
      "wraith",          "Wrait",       15,       {
         120, 100, 100, 100       }
      ,
      {
         "meditation"       }
      ,
      {
         13, 16, 16, 14, 14       }
      , {
         17, 20, 20, 16, 18       }
      , SIZE_MEDIUM
      }*/
   

};


/* used to provide nice verbose wound messages for particular types of
   damage e.g. acid */

const struct dam_string_type dam_string_table[] = {
	/* format: first value is the damage amount, followed by
	   single plural strings for all types of damage */

	{ 0, 
	  {
		  "miss",   0,   0,   0,    0,   0, 
		  0,   0,   0,   0,    0,   0, 
		  0,   0,   0,   0,    0,   0 }
	},
	
	{ 1,
	  {
	  "scrape",   0,   0,   0,    "touch",   0, 
	  "touch",   "brush",   0,   "taint",    "touch",   0, 
	  0,   0,   0,   0,    0,   0 }
	},

	{ 2,
	  {
	  "scratch",   0,   0,   0,    0,   0, 
	  0,   0,   0,   0,    0,   0, 
	  0,   0,   0,   0,    0,   0 }
	},
	{ 3,
	  {
	  "bruise",   0,   0,   0,    "tingle",   "tingle", 
	  "tingle",   "tingle",   0,   "whisp",    "whisp",   0, 
	  0,   0,   0,   0,    0,   0 }
	},
	{ 4,
	  {
	  "graze",   0,   0,   0,    "sting",   "cool", 
	  "spark",   "gnaw",   0,   "strain",   "stir",   0, 
	  0,   0,   0,   0,    0,   0 }
	},
	{ 5,
	  {
	  "hit",   0,   0,   0,    "scald",   "chill", 
	  "zap",   "sting",   0,   "score",    "smolder",   0, 
	  0,   0,   0,   0,    0,   0 }
	},
	{ 6,
	  {
	  "cut",   0,   0,   0,    "blister",   "ice", 
	  "shock",   "blister",   0,   "pain",    "pain",   0, 
	  0,   0,   0,   0,    0,   0 }
	},
	{ 7,
	  {
	  "injure",   0,   0,   0,  "burn",   "frost", 
	  "stun",   "scald",   0,   "palpitate",    "palpitate",   0, 
	  0,   0,   0,   0,    0,   0 }
	},
	{ 8,
	  {
	  "wound",   0,   0,   0,    "torch",   "freeze", 
	  "electrify",   "boil",   0,   "torture",    "torture",   0, 
	  0,   0,   0,   0,    0,   0 }
	},
	{ 10,
	  {
	  "split",   0,   0,   0,    "sizzle",   "solidify", 
	  "electrocute",   "smack",   0,   "agonize",    "agonize",   0, 
	  0,   0,   0,   0,    0,   0 }
	},
	{ 12,
	  {
	  "gash",   0,   0,   0,    "frazzle",   "crackle", 
	  "frazzle",   "deteriorate",   0,  "crush",  "purge",   0, 
	  0,   0,   0,   0,    0,   0 }
	},
	{ 14,
	  {
	  "shred",   0,   0,   0,    "crisp",   "shatter", 
	  "crisp",   "perforate",   0,   "dirty",    "cleanse",   0, 
	  0,   0,   0,   0,    0,   0 }
	},
	{ 16,
	  {
	  "maul",   0,   0,   0,    "cook",   "crystalize", 
	  "impale",   "dissipate",   0,   "ravish",    "pacify",   0, 
	  0,   0,   0,   0,    0,   0 }
	},

	{ 18,
	  {
	  "decimate",   0,   0,   0,    "grill",   0, 
	  "scorch",   "splutter",   0,   "blacken",    "purify",   0, 
	  0,   0,   0,   0,    0,   0 }
	},

	{ 22,
	  {
	  "eviscerate",   0,   0,   0,    "roast",   0, 
	  "sear",   "frizzle",   0,   "curse",    "redeem",  0, 
	  0,   0,   0,   0,    0,   0 }
	},

	{ 26,
	  {
	  "devastate",   0,   0,   0,    "sear",   0, 
	  "cook",   "spatter",   0,   "ruin",    "crystalize",   0, 
	  0,   0,   0,   0,    0,   0 }
	},
	{ 30,
	  {
	  "maim",   0,   0,   0,    "char",   0, 
	  "roast",   "sear",   0,   "enslave",    "spiritualize",   0, 
	  0,   0,   0,   0,    0,   0 }
	},
	{ 35,
	  {
	  "MUTILATE",   0,   0,   0,    "BRAND",   "FREEZE", 
	  "ELECTRIFY",   "DISSOLVE",   0,   0,    "VITRIFY",   0, 
	  0,   0,   0,   0,    0,   0 }
	},

	{ 40,
	  {
	  "DISEMBOWEL",   0,   0,   0,    "MELT",   0, 
	  "ELECTROCUTE",   "MELT",   0,   "OUTCAST",    "CHASTISE",   0, 
	  0,   0,   0,   0,    0,   0 }
	},

	{ 45,
	  {
	  "DISMEMBER",   0,   0,   0,    "ENGULF",   0, 
	  "EMBLAZON",   "DECOMPOSE",   0,   "CRUCIFY",   "CRUCIFY",   0, 
	  0,   0,   0,   0,    0,   0 }
	},

	{ 50,
	  {
	  "MASSACRE",   0,   0,   0,     "NUKE",   0, 
	  "NUKE",   "DISINTEGRATE",   0,   "CONDEMN", "CONDEMN",   0, 
	  0,   0,   0,   0,    0,   0 }
	},
	{ 56,
	  {
	  "MANGLE",   0,   0,   0,   "ATOMIZE",   0, 
	  0,   "ATOMIZE",   0,   0,    0,   0, 
	  0,   0,   0,   0,    0,   0 }
	},

	{ 62,
	  {
	  "DEMOLISH",   0,   0,   0,    0,   0, 
	  0,   0,   0,   0,    0,   0, 
	  0,   0,   0,   0,    0,   0 }
	},

	{ 70,
	  {
	  "DEVASTATE",   0,   0,   0,    0,   0, 
	  0,   0,   0,   0,    0,   0, 
	  0,   0,   0,   0,    0,   0 }
	},

	{ 80,
	  {
	  "OBLITERATE",   0,   0,   0,    0,   0, 
	  0,   0,   0,   0,    0,   0, 
	  0,   0,   0,   0,    0,   0 }
	},

	{ 90,
	  {
	  "ANNIHILATE",   0,   0,   0,    0,   0, 
	  0,   0,   0,   0,    0,   0, 
	  0,   0,   0,   0,    0,   0 }
	},

	{ 120,
	  {
	  "ERADICATE",   0,   0,   0,    0,   0, 
	  0,   0,   0,   0,    0,   0, 
	  0,   0,   0,   0,    0,   0 }
	},

	{ 120, /* leave last item in list null */
	  {
	  '\0',   0,   0,   0,    0,   0, 
	  0,   0,   0,   0,    0,   0, 
	  0,   0,   0,   0,    0,   0 }
	}
};

/* used to determine the damage message in fight.c */

const struct race_body_type race_body_table[MAX_BODY_PARTS] = {
/*
  format:
  {       BODY_PART, "description", found in a pair?, location }  */
	{
		PART_HEAD, "head", FALSE,  3,
		"$n's severed head plops on the ground.", OBJ_VNUM_SEVERED_HEAD },/* don't move from top of list */
	{
		PART_ARMS, "arm", TRUE,  2,
		"$n's arm is sliced from $s dead body.", OBJ_VNUM_SLICED_ARM },      /* double */
	
	{
		PART_LEGS, "leg", TRUE, 1,
		"$n's leg is sliced from $s dead body.", OBJ_VNUM_SLICED_LEG },      /* double */
	{
		PART_HEART, "chest", FALSE,  2,
		"$n's heart is torn from $s chest.", OBJ_VNUM_TORN_HEART },

	{
		PART_BRAINS, "head", FALSE,  3,
		"$n's head is shattered, and $s brains splash all over you.",
		OBJ_VNUM_BRAINS },
             
	{
		PART_GUTS, "midriff", FALSE,  2, 
		"$n spills $s guts all over the floor.", OBJ_VNUM_GUTS },
	
	{	
		PART_HANDS, "hand", TRUE, 2, 0, 0  },    /* double */
	{
		PART_FEET, "shin", TRUE,  1, 0, 0  },     /* double. No feet! */
	{
		PART_FINGERS, "hand", TRUE, 2, 0, 0  },   /* double */
	{ 
		PART_EAR, "head", FALSE,  3,
		"$n's severed head plops on the ground.", OBJ_VNUM_SEVERED_HEAD },
	{       
		PART_EYE, "head", FALSE,  3,
		"$n's severed head plops on the ground.", OBJ_VNUM_SEVERED_HEAD },
	{
		PART_LONG_TONGUE, "tongue", FALSE,  3,
		"$n's severed head plops on the ground.", OBJ_VNUM_SEVERED_HEAD },
	{
		PART_EYESTALKS, "eye stalks", FALSE,  2, 0 ,0 }, /* weird */
	{
		PART_TENTACLES, "tentacles", FALSE,  2, 0 ,0 },
	{
		PART_FINS, "fin", FALSE,  2, 0, 0 },               
	{
		PART_WINGS, "wings", FALSE,  2,
		"$n's wing is sliced off and lands with a crunch.", OBJ_VNUM_SLICED_WING },    
	{
		PART_TAIL, "tail", FALSE,  1, 0, 0 },               
	{ 
		PART_CLAWS, "claws", FALSE,  2,
		"$n's claw flies off and narrowly misses you.", OBJ_VNUM_SLICED_CLAW },
	{
		PART_FANGS, "fangs", FALSE,  3, 0, 0 },              
	{
		PART_HORNS, "horn", FALSE,  3, 0, 0 },              
	{
		PART_SCALES, "scales", FALSE,  2,
		"$n's heart is torn from $s chest.", OBJ_VNUM_TORN_HEART },
	{
		PART_TUSKS, "tusk", FALSE,  2, 0 ,0 }
};              

/*
 * Class table.
 */
const struct   class_type  class_table [MAX_CLASS] =
{
   {
      "mage", "Mag",  STAT_INT,  OBJ_VNUM_SCHOOL_DAGGER,
      {
         3018, 9618       }
      ,  75,  18, 6,  6,  8, 10,
      "mage basics", "mage default"
   }
   ,

   {
      "cleric", "Cle",  STAT_WIS,  OBJ_VNUM_SCHOOL_MACE,
      {
         3003, 9619       }
      ,  75,  18, 2,  7, 10, 8,
      "cleric basics", "cleric default"
   }
   ,

   {
      "knight", "Kni",  STAT_STR,  OBJ_VNUM_SCHOOL_SWORD,
      {
         3028, 9639       }
      ,  75,  18,  -8,  10, 14, 6,
      "knight basics", "knight default"
   }
   ,

   {
      "barbarian", "Bar",  STAT_CON,  OBJ_VNUM_SCHOOL_AXE,
      {
         3022, 9633       }
      ,  75,  16,  -15,  13, 17, 3,
      "barbarian basics", "barbarian default"
   }
};



/*
 * Titles.
 */
char *   const       title_table [MAX_CLASS][MAX_LEVEL+1][2] =
{
   {
      {
         "Man",         "Woman"                 }
      ,

      {
         "Apprentice of Magic",  "Apprentice of Magic"            }
      ,
      {
         "Apprentice of Magic",  "Apprentice of Magic"            }
      ,
      {
         "Apprentice of Magic",  "Apprentice of Magic"            }
      ,
      {
         "Spell Student",     "Spell Student"               }
      ,
      {
         "Spell Student",     "Spell Student"               }
      ,

      {
         "Spell Student",     "Spell Student"               }
      ,
      {
         "Scholar of Magic",     "Scholar of Magic"            }
      ,
      {
         "Scholar of Magic",     "Scholar of Magic"            }
      ,
      {
         "Scholar of Magic",     "Scholar of Magic"            }
      ,
      {
         "Delver in Spells",     "Delveress in Spells"            }
      ,

      {
         "Delver in Spells",     "Delveress in Spells"            }
      ,
      {
         "Delver in Spells",     "Delveress in Spells"            }
      ,
      {
         "Medium of Magic",      "Medium of Magic"          }
      ,
      {
         "Medium of Magic",      "Medium of Magic"          }
      ,
      {
         "Medium of Magic",      "Medium of Magic"          }
      ,

      {
         "Scribe of Magic",      "Scribess of Magic"           }
      ,
      {
         "Scribe of Magic",      "Scribess of Magic"           }
      ,
      {
         "Scribe of Magic",      "Scribess of Magic"           }
      ,
      {
         "Seer",        "Seeress"               }
      ,
      {
         "Seer",        "Seeress"               }
      ,

      {
         "Seer",        "Seeress"               }
      ,
      {
         "Sage",        "Sage"                  }
      ,
      {
         "Sage",        "Sage"                  }
      ,
      {
         "Sage",        "Sage"                  }
      ,
      {
         "Illusionist",    "Illusionist"              }
      ,

      {
         "Illusionist",    "Illusionist"              }
      ,
      {
         "Illusionist",    "Illusionist"              }
      ,
      {
         "Abjurer",        "Abjuress"              }
      ,
      {
         "Abjurer",        "Abjuress"              }
      ,
      {
         "Abjurer",        "Abjuress"              }
      ,

      {
         "Invoker",        "Invoker"               }
      ,
      {
         "Invoker",        "Invoker"               }
      ,
      {
         "Invoker",        "Invoker"               }
      ,
      {
         "Enchanter",         "Enchantress"              }
      ,
      {
         "Enchanter",         "Enchantress"              }
      ,

      {
         "Enchanter",         "Enchantress"              }
      ,
      {
         "Conjurer",       "Conjuress"             }
      ,
      {
         "Conjurer",       "Conjuress"             }
      ,
      {
         "Conjurer",       "Conjuress"             }
      ,
      {
         "Magician",       "Witch"                 }
      ,

      {
         "Magician",       "Witch"                 }
      ,
      {
         "Magician",       "Witch"                 }
      ,
      {
         "Creator",        "Creator"               }
      ,
      {
         "Creator",        "Creator"               }
      ,
      {
         "Creator",        "Creator"               }
      ,

      {
         "Savant",         "Savant"             }
      ,
      {
         "Savant",         "Savant"             }
      ,
      {
         "Savant",         "Savant"             }
      ,
      {
         "Magus",       "Craftess"              }
      ,
      {
         "Magus",       "Craftess"              }
      ,

      {
         "Magus",       "Craftess"              }
      ,
      {
         "Wizard",         "Wizard"             }
      ,
      {
         "Wizard",         "Wizard"             }
      ,
      {
         "Wizard",         "Wizard"             }
      ,
      {
         "Warlock",        "War Witch"             }
      ,

      {
         "Warlock",        "War Witch"             }
      ,
      {
         "Warlock",        "War Witch"             }
      ,
      {
         "Sorcerer",       "Sorceress"             }
      ,
      {
         "Sorcerer",       "Sorceress"             }
      ,
      {
         "Sorcerer",       "Sorceress"             }
      ,

      {
         "Elder Sorcerer",    "Elder Sorceress"          }
      ,
      {
         "Elder Sorcerer",    "Elder Sorceress"          }
      ,
      {
         "Elder Sorcerer",    "Elder Sorceress"          }
      ,
      {
         "Grand Sorcerer",    "Grand Sorceress"          }
      ,
      {
         "Grand Sorcerer",    "Grand Sorceress"          }
      ,

      {
         "Great Sorcerer",    "Great Sorceress"          }
      ,
      {
         "Great Sorcerer",    "Great Sorceress"          }
      ,
      {
         "Great Sorcerer",    "Great Sorceress"          }
      ,
      {
         "Golem Maker",    "Golem Maker"              }
      ,
      {
         "Golem Maker",    "Golem Maker"              }
      ,

      {
         "Greater Golem Maker",  "Greater Golem Maker"            }
      ,
      {
         "Greater Golem Maker",  "Greater Golem Maker"            }
      ,
      {
         "Maker of Stones",      "Maker of Stones"          }
      ,
      {
         "Maker of Stones",      "Maker of Stones"          }
      ,
      {
         "Maker of Potions",     "Maker of Potions"            }
      ,

      {
         "Maker of Potions",     "Maker of Potions"            }
      ,
      {
         "Maker of Scrolls",     "Maker of Scrolls"            }
      ,
      {
         "Maker of Scrolls",     "Maker of Scrolls"            }
      ,
      {
         "Maker of Wands",    "Maker of Wands"           }
      ,
      {
         "Maker of Wands",    "Maker of Wands"           }
      ,

      {
         "Maker of Staves",      "Maker of Staves"          }
      ,
      {
         "Maker of Staves",      "Maker of Staves"          }
      ,
      {
         "Demon Summoner",    "Demon Summoner"           }
      ,
      {
         "Demon Summoner",    "Demon Summoner"           }
      ,
      {
         "Greater Demon Summoner",  "Greater Demon Summoner"         }
      ,

      {
         "Greater Demon Summoner",  "Greater Demon Summoner"         }
      ,
      {
         "Dragon Charmer",    "Dragon Charmer"           }
      ,
      {
         "Dragon Charmer",    "Dragon Charmer"           }
      ,
      {
         "Greater Dragon Charmer",  "Greater Dragon Charmer"         }
      ,
      {
         "Master of all Magic",  "Master of all Magic"            }
      ,

      {
         "Master Mage",    "Master Mage"              }
      ,
      {
         "Avatar of Magic",      "Avatar of Magic"          }
      ,
      {
         "Angel of Magic",    "Angel of Magic"           }
      ,
      {
         "Demigod of Magic",     "Demigoddess of Magic"           }
      ,
      {
         "Immortal of Magic",    "Immortal of Magic"           }
      ,

      {
         "God of Magic",      "Goddess of Magic"            }
      ,
      {
         "Deity of Magic",    "Deity of Magic"           }
      ,
      {
         "Supremity of Magic",      "Supremity of Magic"          }
      ,
      {
         "Creator",        "Creator"               }
      ,
      {
         "Implementor",    "Implementress"               }
   }
   ,

   {
      {
         "Man",         "Woman"                 }
      ,

      {
         "Believer",       "Believer"              }
      ,
      {
         "Believer",       "Believer"              }
      ,
      {
         "Believer",       "Believer"              }
      ,
      {
         "Attendant",         "Attendant"             }
      ,
      {
         "Attendant",         "Attendant"             }
      ,

      {
         "Attendant",         "Attendant"             }
      ,
      {
         "Acolyte",        "Acolyte"               }
      ,
      {
         "Acolyte",        "Acolyte"               }
      ,
      {
         "Acolyte",        "Acolyte"               }
      ,
      {
         "Novice",         "Novice"             }
      ,

      {
         "Novice",         "Novice"             }
      ,
      {
         "Novice",         "Novice"             }
      ,
      {
         "Missionary",        "Missionary"               }
      ,
      {
         "Missionary",        "Missionary"               }
      ,
      {
         "Missionary",        "Missionary"               }
      ,

      {
         "Adept",       "Adept"                 }
      ,
      {
         "Adept",       "Adept"                 }
      ,
      {
         "Adept",       "Adept"                 }
      ,
      {
         "Deacon",         "Deaconess"             }
      ,
      {
         "Deacon",         "Deaconess"             }
      ,

      {
         "Deacon",         "Deaconess"             }
      ,
      {
         "Vicar",       "Vicaress"              }
      ,
      {
         "Vicar",       "Vicaress"              }
      ,
      {
         "Vicar",       "Vicaress"              }
      ,
      {
         "Priest",         "Priestess"             }
      ,

      {
         "Priest",         "Priestess"             }
      ,
      {
         "Priest",         "Priestess"             }
      ,
      {
         "Priest",         "Priestess"             }
      ,
      {
         "Minister",       "Lady Minister"               }
      ,
      {
         "Minister",       "Lady Minister"               }
      ,

      {
         "Minister",       "Lady Minister"               }
      ,
      {
         "Canon",       "Canon"                 }
      ,
      {
         "Canon",       "Canon"                 }
      ,
      {
         "Canon",       "Canon"                 }
      ,
      {
         "Levite",         "Levitess"              }
      ,

      {
         "Levite",         "Levitess"              }
      ,
      {
         "Levite",         "Levitess"              }
      ,
      {
         "Curate",         "Curess"             }
      ,
      {
         "Curate",         "Curess"             }
      ,
      {
         "Curate",         "Curess"             }
      ,

      {
         "Monk",        "Nun"                }
      ,
      {
         "Monk",        "Nun"                }
      ,
      {
         "Monk",        "Nun"                }
      ,
      {
         "Healer",         "Healess"               }
      ,
      {
         "Healer",         "Healess"               }
      ,

      {
         "Healer",         "Healess"               }
      ,
      {
         "Chaplain",       "Chaplain"              }
      ,
      {
         "Chaplain",       "Chaplain"              }
      ,
      {
         "Chaplain",       "Chaplain"              }
      ,
      {
         "Expositor",         "Expositress"              }
      ,

      {
         "Expositor",         "Expositress"              }
      ,
      {
         "Expositor",         "Expositress"              }
      ,
      {
         "Bishop",         "Bishop"             }
      ,
      {
         "Bishop",         "Bishop"             }
      ,
      {
         "Bishop",         "Bishop"             }
      ,

      {
         "Arch Bishop",    "Arch Lady of the Church"        }
      ,
      {
         "Arch Bishop",    "Arch Lady of the Church"        }
      ,
      {
         "Arch Bishop",    "Arch Lady of the Church"        }
      ,
      {
         "Patriarch",         "Matriarch"             }
      ,
      {
         "Patriarch",         "Matriarch"             }
      ,

      {
         "Patriarch",         "Matriarch"             }
      ,
      {
         "Elder Patriarch",      "Elder Matriarch"          }
      ,
      {
         "Elder Patriarch",      "Elder Matriarch"          }
      ,
      {
         "Elder Patriarch",      "Elder Matriarch"          }
      ,
      {
         "Grand Patriarch",      "Grand Matriarch"          }
      ,

      {
         "Grand Patriarch",      "Grand Matriarch"          }
      ,
      {
         "Great Patriarch",      "Great Matriarch"          }
      ,
      {
         "Great Patriarch",      "Great Matriarch"          }
      ,
      {
         "Great Patriarch",      "Great Matriarch"          }
      ,
      {
         "Demon Killer",      "Demon Killer"             }
      ,

      {
         "Demon Killer",      "Demon Killer"             }
      ,
      {
         "Demon Killer",      "Demon Killer"             }
      ,
      {
         "Greater Demon Killer", "Greater Demon Killer"           }
      ,
      {
         "Greater Demon Killer", "Greater Demon Killer"           }
      ,
      {
         "Greater Demon Killer", "Greater Demon Killer"           }
      ,

      {
         "Cardinal of the Sea",  "Cardinal of the Sea"            }
      ,
      {
         "Cardinal of the Sea",  "Cardinal of the Sea"            }
      ,
      {
         "Cardinal of the Earth",   "Cardinal of the Earth"          }
      ,
      {
         "Cardinal of the Earth",   "Cardinal of the Earth"          }
      ,
      {
         "Cardinal of the Air",  "Cardinal of the Air"            }
      ,

      {
         "Cardinal of the Air",  "Cardinal of the Air"            }
      ,
      {
         "Cardinal of the Ether",   "Cardinal of the Ether"          }
      ,
      {
         "Cardinal of the Ether",   "Cardinal of the Ether"          }
      ,
      {
         "Cardinal of the Heavens", "Cardinal of the Heavens"        }
      ,
      {
         "Cardinal of the Heavens", "Cardinal of the Heavens"        }
      ,

      {
         "Avatar of an Immortal",   "Avatar of an Immortal"          }
      ,
      {
         "Avatar of a Deity",    "Avatar of a Deity"           }
      ,
      {
         "Avatar of a Supremity",   "Avatar of a Supremity"          }
      ,
      {
         "Avatar of an Implementor",   "Avatar of an Implementor"       }
      ,
      {
         "Master of all Divinity",  "Mistress of all Divinity"       }
      ,

      {
         "Holy Hero",         "Holy Heroine"             }
      ,
      {
         "Holy Avatar",    "Holy Avatar"              }
      ,
      {
         "Angel",       "Angel"                 }
      ,
      {
         "Demigod",        "Demigoddess"              }
      ,
      {
         "Immortal",       "Immortal"              }
      ,

      {
         "God",         "Goddess"               }
      ,
      {
         "Deity",       "Deity"                 }
      ,
      {
         "Supreme Master",    "Supreme Mistress"            }
      ,
      {
         "Creator",                    "Creator"                             }
      ,
      {
         "Implementor",    "Implementress"               }
   }
   ,

   {
      {
         "Man",         "Woman"                 }
      ,

      {
         "Swordpupil",        "Swordpupil"               }
      ,
      {
         "Swordpupil",        "Swordpupil"               }
      ,
      {
         "Swordpupil",        "Swordpupil"               }
      ,
      {
         "Recruit",        "Recruit"               }
      ,
      {
         "Recruit",        "Recruit"               }
      ,

      {
         "Recruit",        "Recruit"               }
      ,
      {
         "Recruit",        "Recruit"               }
      ,
      {
         "Sentry",         "Sentress"              }
      ,
      {
         "Sentry",         "Sentress"              }
      ,
      {
         "Sentry",         "Sentress"              }
      ,

      {
         "Fighter",        "Fighter"               }
      ,
      {
         "Fighter",        "Fighter"               }
      ,
      {
         "Fighter",        "Fighter"               }
      ,
      {
         "Soldier",        "Soldier"               }
      ,
      {
         "Soldier",        "Soldier"               }
      ,

      {
         "Soldier",        "Soldier"               }
      ,
      {
         "Warrior",        "Warrior"               }
      ,
      {
         "Warrior",        "Warrior"               }
      ,
      {
         "Warrior",        "Warrior"               }
      ,
      {
         "Veteran",        "Veteran"               }
      ,

      {
         "Veteran",        "Veteran"               }
      ,
      {
         "Veteran",        "Veteran"               }
      ,
      {
         "Swordsman",         "Swordswoman"              }
      ,
      {
         "Swordsman",         "Swordswoman"              }
      ,
      {
         "Swordsman",         "Swordswoman"              }
      ,

      {
         "Fencer",         "Fenceress"             }
      ,
      {
         "Fencer",         "Fenceress"             }
      ,
      {
         "Fencer",         "Fenceress"             }
      ,
      {
         "Combatant",         "Combatess"             }
      ,
      {
         "Combatant",         "Combatess"             }
      ,

      {
         "Combatant",         "Combatess"             }
      ,
      {
         "Hero",        "Heroine"               }
      ,
      {
         "Hero",        "Heroine"               }
      ,
      {
         "Hero",        "Heroine"               }
      ,
      {
         "Myrmidon",       "Myrmidon"              }
      ,

      {
         "Myrmidon",       "Myrmidon"              }
      ,
      {
         "Swashbuckler",      "Swashbuckleress"          }
      ,
      {
         "Swashbuckler",      "Swashbuckleress"          }
      ,
      {
         "Swashbuckler",      "Swashbuckleress"          }
      ,
      {
         "Mercenary",         "Mercenaress"              }
      ,

      {
         "Mercenary",         "Mercenaress"              }
      ,
      {
         "Mercenary",         "Mercenaress"              }
      ,
      {
         "Swordmaster",    "Swordmistress"               }
      ,
      {
         "Swordmaster",    "Swordmistress"               }
      ,
      {
         "Swordmaster",    "Swordmistress"               }
      ,

      {
         "Lieutenant",        "Lieutenant"               }
      ,
      {
         "Lieutenant",        "Lieutenant"               }
      ,
      {
         "Lieutenant",        "Lieutenant"               }
      ,
      {
         "Champion",       "Lady Champion"               }
      ,
      {
         "Champion",       "Lady Champion"               }
      ,

      {
         "Champion",       "Lady Champion"               }
      ,
      {
         "Dragoon",        "Lady Dragoon"             }
      ,
      {
         "Dragoon",        "Lady Dragoon"             }
      ,
      {
         "Dragoon",        "Lady Dragoon"             }
      ,
      {
         "Cavalier",       "Lady Cavalier"               }
      ,

      {
         "Cavalier",       "Lady Cavalier"               }
      ,
      {
         "Cavalier",       "Lady Cavalier"               }
      ,
      {
         "Knight",         "Lady Knight"              }
      ,
      {
         "Knight",         "Lady Knight"              }
      ,
      {
         "Knight",         "Lady Knight"              }
      ,

      {
         "Grand Knight",      "Grand Knight"             }
      ,
      {
         "Grand Knight",      "Grand Knight"             }
      ,
      {
         "Grand Knight",      "Grand Knight"             }
      ,
      {
         "Master Knight",     "Master Knight"               }
      ,
      {
         "Master Knight",     "Master Knight"               }
      ,

      {
         "Paladin",        "Paladin"               }
      ,
      {
         "Paladin",        "Paladin"               }
      ,
      {
         "Grand Paladin",     "Grand Paladin"               }
      ,
      {
         "Grand Paladin",     "Grand Paladin"               }
      ,
      {
         "Demon Slayer",      "Demon Slayer"             }
      ,

      {
         "Demon Slayer",      "Demon Slayer"             }
      ,
      {
         "Greater Demon Slayer", "Greater Demon Slayer"           }
      ,
      {
         "Greater Demon Slayer", "Greater Demon Slayer"           }
      ,
      {
         "Dragon Slayer",     "Dragon Slayer"               }
      ,
      {
         "Dragon Slayer",     "Dragon Slayer"               }
      ,

      {
         "Greater Dragon Slayer",   "Greater Dragon Slayer"          }
      ,
      {
         "Greater Dragon Slayer",   "Greater Dragon Slayer"          }
      ,
      {
         "Underlord",         "Underlord"             }
      ,
      {
         "Underlord",         "Underlord"             }
      ,
      {
         "Overlord",       "Overlord"              }
      ,

      {
         "Baron of Thunder",     "Baroness of Thunder"            }
      ,
      {
         "Baron of Thunder",     "Baroness of Thunder"            }
      ,
      {
         "Baron of Storms",      "Baroness of Storms"          }
      ,
      {
         "Baron of Storms",      "Baroness of Storms"          }
      ,
      {
         "Baron of Lightning",      "Baroness of Lightning"          }
      ,

      {
         "Baron of Tornadoes",      "Baroness of Tornadoes"          }
      ,
      {
         "Baron of Hurricanes",  "Baroness of Hurricanes"         }
      ,
      {
         "Baron of Meteors",     "Baroness of Meteors"            }
      ,
      {
         "Baron of the Earth",      "Baroness of the Earth"          }
      ,
      {
         "Master Warrior",    "Master Warrior"           }
      ,

      {
         "Knight Hero",    "Knight Heroine"           }
      ,
      {
         "Avatar of War",     "Avatar of War"               }
      ,
      {
         "Angel of War",      "Angel of War"             }
      ,
      {
         "Demigod of War",    "Demigoddess of War"          }
      ,
      {
         "Immortal Warlord",     "Immortal Warlord"            }
      ,

      {
         "God of War",        "God of War"               }
      ,
      {
         "Deity of War",      "Deity of War"             }
      ,
      {
         "Supreme Master of War",   "Supreme Mistress of War"        }
      ,
      {
         "Creator",                    "Creator"                             }
      ,
      {
         "Implementor",    "Implementress"               }
   }
   ,

   {
      {
         "Man",         "Woman"                 }
      ,

      {
         "Axeman",         "Axewoman"              }
      ,
      {
         "Axeman",         "Axewoman"              }
      ,
      {
         "Axeman",         "Axewoman"              }
      ,
      {
         "Rogue",       "Rogue"              }
      ,
      {
         "Rogue",       "Rogue"              }
      ,

      {
         "Rogue",       "Rogue"              }
      ,
      {
         "Nomad",       "Nomad "             }
      ,
      {
         "Nomad",       "Nomad "             }
      ,
      {
         "Nomad",       "Nomad "             }
      ,
      {
         "Strongman",                    "Strongwoman"               }
      ,

      {
         "Strongman",                    "Strongwoman"               }
      ,
      {
         "Strongman",                    "Strongwoman"               }
      ,
      {
         "Bold",        "Bold"                     }
      ,
      {
         "Bold",        "Bold"                     }
      ,
      {
         "Bold",        "Bold"                     }
      ,

      {
         "Renegade",       "Renegade"              }
      ,
      {
         "Renegade",       "Renegade"              }
      ,
      {
         "Renegade",       "Renegade"              }
      ,
      {
         "Cut-throat",        "Cut-throat"               }
      ,
      {
         "Cut-throat",        "Cut-throat"               }
      ,

      {
         "Mighty",         "Mighty"             }
      ,
      {
         "Mighty",         "Mighty"             }
      ,
      {
         "Mighty",         "Mighty"             }
      ,
      {
         "Warrior",        "Warrior"               }
      ,
      {
         "Warrior",        "Warrior"               }
      ,

      {
         "Warrior",        "Warrior"               }
      ,
      {
         "Executioner",    "Executioner"                 }
      ,
      {
         "Executioner",    "Executioner"                 }
      ,
      {
         "Executioner",    "Executioner"                 }
      ,
      {
         "Weapons Crafter",      "Weapons Crafter"                }
      ,

      {
         "Weapons Crafter",      "Weapons Crafter"                }
      ,
      {
         "Weapons Crafter",      "Weapons Crafter"                }
      ,
      {
         "Weapons Master",    "Weapons Mistress"                  }
      ,
      {
         "Weapons Master",    "Weapons Mistress"                  }
      ,
      {
         "Weapons Master",    "Weapons Mistress"                  }
      ,

      {
         "Highwayman",        "Highwaywoman"             }
      ,
      {
         "Highwayman",        "Highwaywoman"             }
      ,
      {
         "Highwayman",        "Highwaywoman"             }
      ,
      {
         "Fearless",            "Fearless"               }
      ,
      {
         "Fearless",            "Fearless"               }
      ,

      {
         "Fearless",            "Fearless"               }
      ,
      {
         "Barbarian",         "Barbarian"                   }
      ,
      {
         "Barbarian",         "Barbarian"                   }
      ,
      {
         "Barbarian",         "Barbarian"                   }
      ,
      {
         "Kinsman",        "Kinswoman"             }
      ,

      {
         "Kinsman",        "Kinswoman"             }
      ,
      {
         "Kinsman",        "Kinswoman"             }
      ,
      {
         "Provider",             "Provider"                 }
      ,
      {
         "Provider",             "Provider"                 }
      ,
      {
         "Provider",             "Provider"                 }
      ,

      {
         "Judicier",       "Judicier"              }
      ,
      {
         "Judicier",       "Judicier"              }
      ,
      {
         "Judicier",       "Judicier"              }
      ,
      {
         "Slayer",         "Slayer"             }
      ,
      {
         "Slayer",         "Slayer"             }
      ,

      {
         "Slayer",         "Slayer"             }
      ,
      {
         "Giant Slayer",   "Giant Slayer"                       }
      ,
      {
         "Giant Slayer",   "Giant Slayer"                       }
      ,
      {
         "Daemon Slayer",   "Daemon Slayer"                       }
      ,
      {
         "Daemon Slayer",   "Daemon Slayer"                   }
      ,

      {
         "Wanderer of the Plains", "Wanderer of the Plains"                   }
      ,
      {
         "Wanderer of the Plains",         "Wanderer of the Plains"         }
      ,
      {
         "Wanderer of the Plains",       "Wanderer of the Plains"              }
      ,
      {
         "Wanderer of the Plains",       "Wanderer of the Plains"              }
      ,

      {
         "Baron of the Plains",       "Baroness of the Plains"             }
      ,
      {
         "Baron of the Plains",       "Baroness of the Plains"             }
      ,
      {
         "Baron of the Plains",       "Baroness of the Plains"             }
      ,
      {
         "Lord of the Plains", "Lady of the Plains"           }
      ,
      {
         "Lord of the Plains", "Lady of the Plains"           }
      ,

      {
         "Blade Master",    "Blade Mistress"          }
      ,
      {
	 "Blade Master",    "Blade Mistress"              }
      ,


      {
         "Infamous",                 "Infamous"                      }
      ,
      {
         "Infamous",                 "Infamous"                      }
      ,

      {
         "Infamous",                 "Infamous"           }
      ,

      {
         "Master of Vision",     "Mistress of Vision"          }
      ,
      {
         "Master of Vision",     "Mistress of Vision"          }
      ,

      {
         "Master of Lore",    "Mistress of Lore"            }
      ,
      {
         "Master of Lore",    "Mistress of Lore"            }
      ,

      {
         "Master of the Hunt",      "Mistress of the Hunt"           }
      ,
      {
         "Master of the Hunt",      "Mistress of the Hunt"           }
      ,

      {
         "Master of the Hunt",      "Mistress of the Hunt"           }
      ,
      {
         "Guardian of an Immortal",      "Guardian of an Immortal"           }
      ,
      {
         "Guardian of an Implementor",   "Guardian of an Implementor"           }
      ,
      {
         "Guardian of an Implementor",    "Guardian of an Implementor"                    }
      ,
      {
         "Master of the Wild",      "Mistress of the Wild"           }
      ,

      {
         "King of the Animals",     "Queen of the Animals"           }
      ,
      {
         "Guardian of Justice",     "Guardian of Justice"            }
      ,
      {
         "Protector of the Planet", "Protector of the Planet"              }
      ,
      {
         "Legend of the Realm",  "Legend of the Realm"               }
      ,
      {
         "Barbarian King",           "Barbarian Queen"                    }
      ,

      {
         "Hero of Barbarity", "Heroine of Barbarity"           }
      ,
      {
         "Avatar of Death",      "Avatar of Death"          }
      ,
      {
         "Angel of Death",    "Angel of Death"           }
      ,
      {
         "Demigod",                  "Demigoddess"                      }
      ,
      {
         "Immortal",             "Immortal"                    }
      ,

      {
         "God of Barbarity",     "Goddess of Barbarity"        }
      ,
      {
         "Deity",             "Deity"                          }
      ,
      {
         "Supreme Master",    "Supreme Mistress"            }
      ,
      {
         "Creator",                    "Creator"                             }
      ,
      {
         "Implementor",    "Implementress"               }
   }
};



/*
 * Attribute bonus tables.
 */
const struct   str_app_type   str_app     [26]     =
{
   {
      -5, -4,   0,  0    }
   ,  /* 0  */
   {
      -5, -4,   3,  1    }
   ,  /* 1  */
   {
      -3, -2,   3,  2    }
   ,
   {
      -3, -1,  10,  3    }
   ,  /* 3  */
   {
      -2, -1,  25,  4    }
   ,
   {
      -2, -1,  55,  5    }
   ,  /* 5  */
   {
      -1,  0,  80,  6    }
   ,
   {
      -1,  0,  90,  7    }
   ,
   {
      0,  0, 100,  8    }
   ,
   {
      0,  0, 100,  9    }
   ,
   {
      0,  0, 115, 10    }
   , /* 10  */
   {
      0,  0, 115, 11    }
   ,
   {
      0,  0, 130, 12    }
   ,
   {
      0,  0, 130, 13    }
   , /* 13  */
   {
      0,  1, 140, 14    }
   ,
   {
      1,  1, 150, 15    }
   , /* 15  */
   {
      1,  2, 165, 16    }
   ,
   {
      2,  3, 180, 22    }
   ,
   {
      2,  3, 200, 25    }
   , /* 18  */
   {
      3,  4, 225, 30    }
   ,
   {
      3,  5, 250, 35    }
   , /* 20  */
   {
      4,  6, 300, 40    }
   ,
   {
      4,  6, 350, 45    }
   ,
   {
      5,  7, 400, 50    }
   ,
   {
      5,  8, 450, 55    }
   ,
   {
      6,  9, 500, 60    }  /* 25   */
};

const struct   int_app_type   int_app     [26]     =
{
   {
      3    }
   ,  /*  0 */
   {
      5    }
   ,  /*  1 */
   {
      7    }
   ,
   {
      8    }
   ,  /*  3 */
   {
      9    }
   ,
   {
      10    }
   ,  /*  5 */
   {
      11    }
   ,
   {
      12    }
   ,
   {
      13    }
   ,
   {
      15    }
   ,
   {
      17    }
   ,  /* 10 */
   {
      19    }
   ,
   {
      22    }
   ,
   {
      25    }
   ,
   {
      28    }
   ,
   {
      31    }
   ,  /* 15 */
   {
      34    }
   ,
   {
      37    }
   ,
   {
      40    }
   ,  /* 18 */
   {
      44    }
   ,
   {
      49    }
   ,  /* 20 */
   {
      55    }
   ,
   {
      60    }
   ,
   {
      70    }
   ,
   {
      80    }
   ,
   {
      85    }  /* 25 */
};



const struct   wis_app_type   wis_app     [26]     =
{
   {
      0    }
   ,  /*  0 */
   {
      0    }
   ,  /*  1 */
   {
      0    }
   ,
   {
      0    }
   ,  /*  3 */
   {
      0    }
   ,
   {
      1    }
   ,  /*  5 */
   {
      1    }
   ,
   {
      1    }
   ,
   {
      1    }
   ,
   {
      1    }
   ,
   {
      1    }
   ,  /* 10 */
   {
      1    }
   ,
   {
      1    }
   ,
   {
      1    }
   ,
   {
      1    }
   ,
   {
      2    }
   ,  /* 15 */
   {
      2    }
   ,
   {
      2    }
   ,
   {
      3    }
   ,  /* 18 */
   {
      3    }
   ,
   {
      3    }
   ,  /* 20 */
   {
      3    }
   ,
   {
      4    }
   ,
   {
      4    }
   ,
   {
      4    }
   ,
   {
      5    }   /* 25 */
};



const struct   dex_app_type   dex_app     [26]     =
{
   {
      60    }
   ,   /* 0 */
   {
      50    }
   ,   /* 1 */
   {
      50    }
   ,
   {
      40    }
   ,
   {
      30    }
   ,
   {
      20    }
   ,   /* 5 */
   {
      10    }
   ,
   {
      0    }
   ,
   {
      0    }
   ,
   {
      0    }
   ,
   {
      0    }
   ,   /* 10 */
   {
      0    }
   ,
   {
      0    }
   ,
   {
      0    }
   ,
   {
      0    }
   ,
   {
      - 10    }
   ,   /* 15 */
   {
      - 15    }
   ,
   {
      - 20    }
   ,
   {
      - 30    }
   ,
   {
      - 40    }
   ,
   {
      - 50    }
   ,   /* 20 */
   {
      - 60    }
   ,
   {
      - 75    }
   ,
   {
      - 90    }
   ,
   {
      -105    }
   ,
   {
      -120    }    /* 25 */
};


const struct   con_app_type   con_app     [26]     =
{
   {
      -4, 20    }
   ,   /*  0 */
   {
      -3, 25    }
   ,   /*  1 */
   {
      -2, 30    }
   ,
   {
      -2, 35    }
   ,    /*  3 */
   {
      -1, 40    }
   ,
   {
      -1, 45    }
   ,   /*  5 */
   {
      -1, 50    }
   ,
   {
      0, 55    }
   ,
   {
      0, 60    }
   ,
   {
      0, 65    }
   ,
   {
      0, 70    }
   ,   /* 10 */
   {
      0, 75    }
   ,
   {
      0, 80    }
   ,
   {
      0, 85    }
   ,
   {
      0, 88    }
   ,
   {
      1, 90    }
   ,   /* 15 */
   {
      2, 95    }
   ,
   {
      2, 97    }
   ,
   {
      3, 99    }
   ,   /* 18 */
   {
      3, 99    }
   ,
   {
      4, 99    }
   ,   /* 20 */
   {
      4, 99    }
   ,
   {
      5, 99    }
   ,
   {
      6, 99    }
   ,
   {
      7, 99    }
   ,
   {
      8, 99    }    /* 25 */
};



/*
 * Liquid properties.
 * Used in world.obj.
 */
const struct   liq_type liq_table   []   =
{
   {
      "water",       "clear", {
         0, 1, 10       }
   }
   ,  /*  0 */
   {
      "beer",        "amber", {
         3, 2,  5       }
   }
   ,
   {
      "wine",        "rose",     {
         5, 2,  5       }
   }
   ,
   {
      "ale",         "brown", {
         2, 2,  5       }
   }
   ,
   {
      "dark ale",    "dark",     {
         1, 2,  5       }
   }
   ,

   {
      "whisky",         "golden",   {
         6, 1,  4       }
   }
   ,  /*  5 */
   {
      "lemonade",    "pink",     {
         0, 1,  8       }
   }
   ,
   {
      "firebreather",      "boiling",  {
         10, 0,  0       }
   }
   ,
   {
      "local specialty",   "everclear",   {
         3, 3,  3       }
   }
   ,
   {
      "slime mold juice",  "green", {
         0, 4, -8       }
   }
   ,

   {
      "milk",        "white", {
         0, 3,  6       }
   }
   ,  /* 10 */
   {
      "tea",         "tan",      {
         0, 1,  6       }
   }
   ,
   {
      "coffee",         "black", {
         0, 1,  6       }
   }
   ,
   {
      "blood",       "red",      {
         0, 2, -1       }
   }
   ,
   {
      "salt water",     "clear", {
         0, 1, -2       }
   }
   ,

   {
      "cola",        "cherry",   {
         0, 1,  5       }
   }   /* 15 */
   ,
   {
      "red wine",        "red",     {
         5, 2,  5       }
   }
   /* 16 */
   ,
   {
	   0}
};



/*
 * The skill and spell table.
 * Slot numbers must never be changed as they appear in #OBJECTS sections.
 */
#define SLOT(n)   n

const struct   skill_type  skill_table [MAX_SKILL] =
{

   /*
    * Magic spells.
    */

   {
      "reserved",    {
         99, 99, 99, 99       }
      ,  {
         99, 99, 99, 99      }
      ,
      0,       TAR_IGNORE,    POS_STANDING,
      NULL,       SLOT( 0),    0,    0,
      "",         ""
   }
   ,

   {
      "acid blast",     {
         28, 60, 60, 60       }
      ,  {
         1,  1,  2,  2      }
      ,
      spell_acid_blast, TAR_CHAR_OFFENSIVE,  POS_FIGHTING,
      NULL,       SLOT(70),   20,   12,
      "acid blast",     "!Acid Blast!"
   }
   ,

   {
      "octarine fire",       {
         40, 60, 60, 60       }
      ,   {
         2,   3,  3,  3      }
      ,
      spell_octarine_fire,   TAR_CHAR_OFFENSIVE,   POS_FIGHTING,
      &gsn_octarine_fire,    SLOT(99),       50,     12,
      "octarine fire",       "The octarine fire earths to the ground."
   }
   ,

   {
      "armor",    {
         7,  2, 3, 5       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_armor,      TAR_CHAR_DEFENSIVE,  POS_STANDING,
      NULL,       SLOT( 1),    5,   12,
      "",         "You feel less protected."
   }
   ,

   {
      "bless",    {
         60,  7, 9, 60       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_bless,      TAR_CHAR_DEFENSIVE,  POS_STANDING,
      &gsn_bless,       SLOT( 3),    5,   12,
      "",         "You feel less righteous."
   }
   ,

   {
      "blindness",      {
         12,  8, 11, 15       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_blindness,  TAR_CHAR_OFFENSIVE,  POS_FIGHTING,
      &gsn_blindness,      SLOT( 4),    5,   12,
      "",         "You can see again."
   }
   ,

   {
      "burning hands",  {
         7, 60, 60, 60       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_burning_hands, TAR_CHAR_OFFENSIVE,  POS_FIGHTING,
      NULL,       SLOT( 5),   15,   12,
      "burning hands",  "!Burning Hands!"
   }
   ,

   {
      "call lightning", {
         26, 18, 22, 29       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_call_lightning,   TAR_IGNORE,    POS_FIGHTING,
      NULL,       SLOT( 6),   15,   12,
      "lightning bolt", "!Call Lightning!"
   }
   ,

   {
      "calm",        {
         60, 16, 20, 60       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_calm,    TAR_IGNORE,    POS_FIGHTING,
      NULL,       SLOT(509),  30,   12,
      "",         "You have lost your peace of mind."
   }
   ,

   {
      "cancellation",      {
         18, 26, 34, 34       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_cancellation,  TAR_CHAR_DEFENSIVE,  POS_FIGHTING,
      NULL,       SLOT(507),  20,   12,
      ""       "!cancellation!",
   }
   ,

   {
      "cause critical", {
         60,  13, 19, 60       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_cause_critical,   TAR_CHAR_OFFENSIVE,  POS_FIGHTING,
      NULL,       SLOT(63),   20,   12,
      "spell",    "!Cause Critical!"
   }
   ,

   {
      "cause light",    {
         60,  1, 3, 60       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_cause_light,   TAR_CHAR_OFFENSIVE,  POS_FIGHTING,
      NULL,       SLOT(62),   15,   12,
      "spell",    "!Cause Light!"
   }
   ,

   {
      "cause serious",  {
         60,  7, 10, 60       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_cause_serious, TAR_CHAR_OFFENSIVE,  POS_FIGHTING,
      NULL,       SLOT(64),   17,   12,
      "spell",    "!Cause Serious!"
   }
   ,

   {
      "chain lightning",   {
         33, 60, 60, 60       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_chain_lightning,  TAR_CHAR_OFFENSIVE,  POS_FIGHTING,
      NULL,       SLOT(500),  45,   14,
      "lightning",      "!Chain Lightning!"
   }
   ,

   {
      "change sex",     {
         60, 60, 60, 60       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_change_sex, TAR_CHAR_DEFENSIVE,  POS_FIGHTING,
      NULL,       SLOT(82),   15,   12,
      "",         "Your body feels familiar again."
   }
   ,

   {
      "charm person",      {
         20, 60, 25, 60       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_charm_person,  TAR_CHAR_OFFENSIVE,  POS_STANDING,
      &gsn_charm_person,   SLOT( 7),    5,   12,
      "",         "You feel more self-confident."
   }
   ,

   {
      "chill touch",    {
         4, 60, 60, 60       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_chill_touch,   TAR_CHAR_OFFENSIVE,  POS_FIGHTING,
      NULL,       SLOT( 8),   15,   12,
      "chilling touch", "You feel less cold."
   }
   ,

   {
      "colour spray",      {
         16, 60, 60, 60       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_colour_spray,  TAR_CHAR_OFFENSIVE,  POS_FIGHTING,
      NULL,       SLOT(10),   15,   12,
      "colour spray",      "!Colour Spray!"
   }
   ,

   {
      "continual light",   {
         6,  4, 6, 9       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_continual_light,  TAR_IGNORE,    POS_STANDING,
      NULL,       SLOT(57),    7,   12,
      "",         "!Continual Light!"
   }
   ,

   {
      "control weather",   {
         15, 19, 22, 28       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_control_weather,  TAR_IGNORE,    POS_STANDING,
      NULL,       SLOT(11),   25,   12,
      "",         "!Control Weather!"
   }
   ,

   {
      "create food",    {
         10, 5, 11, 11       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_create_food,   TAR_IGNORE,    POS_STANDING,
      NULL,       SLOT(12),    5,   12,
      "",         "!Create Food!"
   }
   ,

   {
      "create spring",  {
         14, 17, 22, 22       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_create_spring, TAR_IGNORE,    POS_STANDING,
      NULL,       SLOT(80),   20,   12,
      "",         "!Create Spring!"
   }
   ,

   {
      "create water",      {
         8,  3, 12, 11       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_create_water,  TAR_OBJ_INV,      POS_STANDING,
      NULL,       SLOT(13),    5,   12,
      "",         "!Create Water!"
   }
   ,

   {
      "cure blindness", {
         60,  6, 8, 10       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_cure_blindness,   TAR_CHAR_DEFENSIVE,  POS_FIGHTING,
      NULL,       SLOT(14),    5,   12,
      "",         "!Cure Blindness!"
   }
   ,

   {
      "cure critical",  {
         60,  13, 17, 60       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_cure_critical, TAR_CHAR_DEFENSIVE,  POS_FIGHTING,
      NULL,       SLOT(15),   20,   12,
      "",         "!Cure Critical!"
   }
   ,

   {
      "cure disease",      {
         60, 13, 14, 17       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_cure_disease,  TAR_CHAR_DEFENSIVE,  POS_STANDING,
      NULL,       SLOT(501),  20,   12,
      "",         "!Cure Disease!"
   }
   ,

   {
      "cure light",     {
         60,  1, 3, 60       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_cure_light, TAR_CHAR_DEFENSIVE,  POS_FIGHTING,
      NULL,       SLOT(16),   10,   12,
      "",         "!Cure Light!"
   }
   ,

   {
      "cure poison",    {
         60,  14, 16, 19       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_cure_poison,   TAR_CHAR_DEFENSIVE,  POS_STANDING,
      NULL,       SLOT(43),    5,   12,
      "",         "!Cure Poison!"
   }
   ,

   {
      "cure serious",      {
         60,  7, 10, 60       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_cure_serious,  TAR_CHAR_DEFENSIVE,  POS_FIGHTING,
      NULL,       SLOT(61),   15,   12,
      "",         "!Cure Serious!"
   }
   ,

   {
      "curse",    {
         18, 18, 26, 22       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_curse,      TAR_CHAR_OFFENSIVE,  POS_FIGHTING,
      &gsn_curse,    SLOT(17),   20,   12,
      "curse",    "The curse wears off."
   }
   ,

   {
      "exorcise",             {
         60, 34, 40, 60       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_exorcise,         TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
      NULL,                   SLOT(506),      20,     12,
      "exorcism",             "!exorcise!"
   }
   ,

   {
      "demonfire",      {
         60, 34, 40, 60       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_demonfire,  TAR_CHAR_OFFENSIVE,  POS_FIGHTING,
      NULL,       SLOT(505),  20,   12,
      "malice",    "!Demonfire!"
   }
   ,

   {
      "detect evil",    {
         12,  4, 60, 60       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_detect_evil,   TAR_CHAR_SELF,    POS_STANDING,
      NULL,       SLOT(18),    5,   12,
      "",         "The red in your vision disappears."
   }
   ,

   {
      "detect hidden",  {
         15, 11, 60, 60       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_detect_hidden, TAR_CHAR_SELF,    POS_STANDING,
      NULL,       SLOT(44),    5,   12,
      "",         "You feel less aware of your suroundings."
   }
   ,

   {
      "detect invis",      {
         3,  8, 60, 60       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_detect_invis,  TAR_CHAR_SELF,    POS_STANDING,
      NULL,       SLOT(19),    5,   12,
      "",         "You no longer see invisible objects."
   }
   ,

   {
      "detect magic",      {
         2,  6, 60, 60       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_detect_magic,  TAR_CHAR_SELF,    POS_STANDING,
      NULL,       SLOT(20),    5,   12,
      "",         "The detect magic wears off."
   }
   ,

   {
      "detect poison",  {
         15,  7, 60, 60       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_detect_poison, TAR_OBJ_INV,      POS_STANDING,
      NULL,       SLOT(21),    5,   12,
      "",         "!Detect Poison!"
   }
   ,

   {
      "dispel evil",    {
         60, 15, 20, 60       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_dispel_evil,   TAR_CHAR_OFFENSIVE,  POS_FIGHTING,
      NULL,       SLOT(22),   15,   12,
      "dispel evil",    "!Dispel Evil!"
   }
   ,

   {
      "dispel good",          {
         60, 15, 20, 60       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_dispel_good,      TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
      NULL,                   SLOT(322),       15,     12,
      "dispel good",          "!Dispel Good!"
   }
   ,

   {
      "dispel magic",      {
         16, 24, 30, 30       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_dispel_magic,  TAR_CHAR_OFFENSIVE,  POS_FIGHTING,
      NULL,       SLOT(59),   15,   12,
      "",         "!Dispel Magic!"
   }
   ,

   {
      "earthquake",     {
         60,  10, 14, 60      }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_earthquake, TAR_IGNORE,    POS_FIGHTING,
      NULL,       SLOT(23),   15,   12,
      "earthquake",     "!Earthquake!"
   }
   ,

   {
      "enchant armor",  {
         16, 60, 60, 60       }
      ,  {
         1,  1,  2,  2       }
      ,
      spell_enchant_armor, TAR_OBJ_INV,      POS_STANDING,
      NULL,       SLOT(510),  100,  24,
      "",         "!Enchant Armor!"
   }
   ,

   {
      "protect container",       {
         50, 50, 50, 50       }
      ,      {
         1,  1,  2,  2       }
      ,
      spell_protect_container,   TAR_OBJ_INV,             POS_STANDING,
      NULL,                      SLOT (606),     50,      24,
      "",                        "!Protect Container!"
   }
   ,

   {
      "venom",        {
         60, 60, 60, 10       }
      ,     {
         1,  1,  2,  1       }
      ,
      spell_venom,    TAR_OBJ_INV,            POS_STANDING,
      NULL,                   SLOT(600),      50,    24,
      "",                     "!Venom!"
   }
   ,

   {
      "vorpal",        {
         93, 93, 93, 93       }
      ,     {
         1,  1,  1,  1       }
      ,
      spell_vorpal,    TAR_OBJ_INV,            POS_STANDING,
      NULL,                   SLOT(608),      50,    24,
      "",                     "!Venom!"
   }
   ,

   {
      "vampire",      {
         93, 93, 93, 93       }
      ,     {
         1,  1,  2,  1       }
      ,
      spell_vampire,  TAR_OBJ_INV,            POS_STANDING,
      NULL,                   SLOT(605),      50,    24,
      "",                     "!Vampire!"
   }
   ,

   {
      "black death",        {
         60, 60, 60, 35       }
      ,     {
         2,  2,  2,  1       }
      ,
      spell_black_death,    TAR_OBJ_INV,            POS_STANDING,
      NULL,                   SLOT(601),      100,    24,
      "",                     "!Black Death!"
   }
   ,

   {
      "tame lightning",        {
         60, 60, 60, 20       }
      ,     {
         2,  2,  2,  1       }
      ,
      spell_tame_lightning,    TAR_OBJ_INV,            POS_STANDING,
      NULL,                   SLOT(602),      75,    24,
      "",                     "!Tame Lightning!"
   }
   ,

   {
      "acid wash",        {
         60, 60, 60, 45       }
      ,     {
         2,  2,  2,  1       }
      ,
      spell_acid_wash,    TAR_OBJ_INV,            POS_STANDING,
      NULL,                   SLOT(603),      150,    24,
      "",                     "!Acid Wash!"
   }
   ,

   {
      "damnation",        {
         60, 60, 60, 50       }
      ,     {
         2,  2,  2,  1       }
      ,
      spell_damnation,    TAR_OBJ_INV,            POS_STANDING,
      NULL,                   SLOT(604),      200,    24,
      "",                     "!Damnation!"
   }
   ,

   {
      "enchant weapon", {
         17, 60, 60, 60       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_enchant_weapon,   TAR_OBJ_INV,      POS_STANDING,
      NULL,       SLOT(24),   100,  24,
      "",         "!Enchant Weapon!"
   }
   ,

   {
      "energy drain",      {
         19, 22, 26, 23       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_energy_drain,  TAR_CHAR_OFFENSIVE,  POS_FIGHTING,
      NULL,       SLOT(25),   35,   12,
      "energy drain",      "!Energy Drain!"
   }
   ,

   {
      "insanity",    {
         16, 18, 23, 20       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_insanity,           TAR_CHAR_OFFENSIVE,   POS_FIGHTING,
      &gsn_insanity,       SLOT(206),  35,   12,
      "insanity",    "Your mind clears."
   }
   ,

   {
      "faerie fire",    {
         6,  3, 5, 8       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_faerie_fire,   TAR_CHAR_OFFENSIVE,  POS_FIGHTING,
      NULL,       SLOT(72),    5,   12,
      "faerie fire",    "The pink aura around you fades away."
   }
   ,

   {
      "faerie fog",     {
         14, 21, 16, 24       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_faerie_fog, TAR_IGNORE,    POS_STANDING,
      NULL,       SLOT(73),   12,   12,
      "faerie fog",     "!Faerie Fog!"
   }
   ,

   {
      "fireball",    {
         22, 60, 60, 60       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_fireball,      TAR_CHAR_OFFENSIVE,  POS_FIGHTING,
      NULL,       SLOT(26),   15,   12,
      "fireball",    "!Fireball!"
   }
   ,

   {
      "flamestrike",    {
         60, 20, 27, 60       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_flamestrike,   TAR_CHAR_OFFENSIVE,  POS_FIGHTING,
      NULL,       SLOT(65),   20,   12,
      "flamestrike",    "!Flamestrike!"
   }
   ,

   {
      "fly",         {
         10, 18, 20, 22       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_fly,     TAR_CHAR_DEFENSIVE,  POS_STANDING,
      NULL,       SLOT(56),   10,   18,
      "",         "You slowly float to the ground."
   }
   ,

   {
      "frenzy",               {
         60, 24, 26, 60       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_frenzy,           TAR_CHAR_DEFENSIVE,     POS_STANDING,
      NULL,                   SLOT(504),      30,     24,
      "",                     "Your rage ebbs."
   }
   ,

   {
      "gate",        {
         27, 17, 28, 28       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_gate,    TAR_IGNORE,    POS_FIGHTING,
      NULL,       SLOT(83),   80,   12,
      "",         "!Gate!"
   }
   ,

   {
      "giant strength", {
         11, 19, 22, 16       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_giant_strength,   TAR_CHAR_DEFENSIVE,  POS_STANDING,
      NULL,       SLOT(39),   20,   12,
      "",         "You feel weaker."
   }
   ,

   {
      "harm",        {
         60, 23, 26, 60       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_harm,    TAR_CHAR_OFFENSIVE,  POS_FIGHTING,
      NULL,       SLOT(27),   35,   12,
      "harm spell",     "!Harm!"
   }
   ,

   {
      "haste",    {
         21, 29, 27, 26       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_haste,      TAR_CHAR_DEFENSIVE,  POS_FIGHTING,
      NULL,       SLOT(502),  30,   12,
      "",         "You feel yourself slow down."
   }
   ,

   {
      "regeneration",         {
         17, 21, 20, 19      }
      ,       {
         1,  1,  2,  2      }
      ,
      spell_regeneration,     TAR_CHAR_DEFENSIVE,     POS_STANDING,
      NULL,                   SLOT(511),      12 ,     12,
      "You feel vibrant!",    "You feel less vibrant."
   }
   ,

   {
      "lethargy",             {
         21, 29, 27, 26       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_lethargy,         TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
      NULL,                   SLOT(208),      30,     12,
      "",                     "Your heart-beat quickens."
   }
   ,

   {
      "heal",        {
         60, 21, 29, 60       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_heal,    TAR_CHAR_DEFENSIVE,  POS_FIGHTING,
      NULL,       SLOT(28),   50,   12,
      "",         "!Heal!"
   }
   ,

   {
      "holy word",      {
         60, 45, 50, 60       }
      ,  {
         2,  2,  4,  4      }
      ,
      spell_holy_word,  TAR_IGNORE, POS_FIGHTING,
      NULL,       SLOT(506),  100,  24,
      "divine wrath",      "!Holy Word!"
   }
   ,

   {
      "identify",    {
         15, 16, 60, 60       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_identify,      TAR_OBJ_INV,      POS_STANDING,
      NULL,       SLOT(53),   12,   24,
      "",         "!Identify!"
   }
   ,

   {
      "infravision",    {
         9,  13, 15, 11       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_infravision,   TAR_CHAR_DEFENSIVE,  POS_STANDING,
      NULL,       SLOT(77),    5,   18,
      "",         "You no longer see in the dark."
   }
   ,

   {
      "invis",    {
         5, 60, 60, 60       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_invis,      TAR_CHAR_DEFENSIVE,  POS_STANDING,
      &gsn_invis,    SLOT(29),    5,   12,
      "",         "You are no longer invisible."
   }
   ,

   {
      "know alignment", {
         12,  9, 60, 60       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_know_alignment,   TAR_CHAR_DEFENSIVE,  POS_FIGHTING,
      NULL,       SLOT(58),    9,   12,
      "",         "!Know Alignment!"
   }
   ,

   {
      "lightning bolt", {
         13, 23, 18, 22       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_lightning_bolt,   TAR_CHAR_OFFENSIVE,  POS_FIGHTING,
      NULL,       SLOT(30),   15,   12,
      "lightning bolt", "!Lightning Bolt!"
   }
   ,

   {
      "locate object",  {
         9, 15, 60, 60       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_locate_object, TAR_IGNORE,    POS_STANDING,
      NULL,       SLOT(31),   20,   18,
      "",         "!Locate Object!"
   }
   ,

   {
      "magic missile",  {
         1, 60, 60, 60       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_magic_missile, TAR_CHAR_OFFENSIVE,  POS_FIGHTING,
      NULL,       SLOT(32),   15,   12,
      "magic missile",  "!Magic Missile!"
   }
   ,

   {
      "mass healing",      {
         60, 38, 45, 60       }
      ,  {
         2,  2,  4,  4      }
      ,
      spell_mass_healing,  TAR_IGNORE,    POS_STANDING,
      NULL,       SLOT(508),  100,  36,
      "",         "!Mass Healing!"
   }
   ,

   {
      "mass invis",     {
         22, 60, 60, 60       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_mass_invis, TAR_IGNORE,    POS_STANDING,
      &gsn_mass_invis,  SLOT(69),   20,   24,
      "",         "You are no longer invisible."
   }
   ,

   {
      "pass door",      {
         24, 30, 28, 34       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_pass_door,  TAR_CHAR_DEFENSIVE,  POS_STANDING,
      NULL,       SLOT(74),   20,   12,
      "",         "You feel solid again."
   }
   ,

   {
      "plague",      {
         23, 17, 36, 26       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_plague,     TAR_CHAR_OFFENSIVE,  POS_FIGHTING,
      &gsn_plague,      SLOT(503),  20,   12,
      "sickness",    "Your sores vanish."
   }
   ,

   {
      "poison",      {
         17,  12, 15, 21       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_poison,     TAR_CHAR_OFFENSIVE,  POS_FIGHTING,
      &gsn_poison,      SLOT(33),   10,   12,
      "poison",      "You feel less sick."
   }
   ,

   {
      "portal",               {
         40,  40, 40, 40       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_portal,           TAR_IGNORE,             POS_STANDING,
      NULL,                   SLOT(516),      100,     12,
      "",                     ""
   }
   ,

   {
      "protection evil",   {
         12,  9, 12, 12       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_protection_evil,  TAR_CHAR_SELF,    POS_STANDING,
      NULL,       SLOT(34),    5,   12,
      "",         "You feel less protected from evil."
   }
   ,

   {
      "protection good",   {
         12,  9, 12, 12       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_protection_good,  TAR_CHAR_SELF,    POS_STANDING,
      NULL,       SLOT(607),   5,   12,
      "",         "You feel less protected from good."
   }
   ,

   {
      "psychic tornado",   {
         60, 50, 70, 70       }
      ,  {
         1,  1,  2,  2      }
      ,
      spell_psy_tornado,   TAR_CHAR_OFFENSIVE,  POS_STANDING,
      NULL,       SLOT(207),     100,  16,
      "psychic tornado",      "You regain your senses."
   }
   ,

   {
      "refresh",     {
         8,  5, 12, 9       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_refresh,    TAR_CHAR_DEFENSIVE,  POS_STANDING,
      NULL,       SLOT(81),   12,   18,
      "refresh",     "!Refresh!"
   }
   ,

   {
      "remove curse",      {
         60, 18, 40, 60       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_remove_curse,  TAR_CHAR_DEFENSIVE,  POS_STANDING,
      NULL,       SLOT(35),    5,   12,
      "",         "!Remove Curse!"
   }
   ,

   {
      "remove alignment",      {
         60, 18, 60, 60       }
      ,    {
         1,  1,  2,  2      }
      ,
      spell_remove_alignment,  TAR_OBJ_INV,           POS_STANDING,
      NULL,                    SLOT(512),         100,    12,
      "",                      "!Remove Alignment!"
   }
   ,

   {
      "remove invisible",      {
         60, 18, 60, 60       }
      ,    {
         1,  1,  2,  2      }
      ,
      spell_remove_invisible,  TAR_OBJ_INV,           POS_STANDING,
      NULL,                    SLOT(513),         30,    12,
      "",                      "!Remove Invisible!"
   }
   ,

   {
      "talon",            {
         40, 40, 40, 40       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_talon,        TAR_CHAR_DEFENSIVE,     POS_STANDING,
      NULL,                   SLOT(37),       28,     12,
      "",                     "Your hands return to normal."
   }
   ,

   {
      "sanctuary",      {
         36, 20, 30, 37       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_sanctuary,  TAR_CHAR_DEFENSIVE,  POS_STANDING,
      NULL,       SLOT(36),   75,   12,
      "",         "The white aura around your body fades."
   }
   ,

   {
      "shield",      {
         20, 35, 35, 34       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_shield,     TAR_CHAR_DEFENSIVE,  POS_STANDING,
      NULL,       SLOT(67),   12,   18,
      "",         "Your force shield shimmers then fades away."
   }
   ,

   {
      "shocking grasp", {
         10, 60, 60, 60       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_shocking_grasp,   TAR_CHAR_OFFENSIVE,  POS_FIGHTING,
      NULL,       SLOT(53),   15,   12,
      "shocking grasp", "!Shocking Grasp!"
   }
   ,

   {
      "sleep",    {
         10, 60, 60, 60       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_sleep,      TAR_CHAR_OFFENSIVE,  POS_STANDING,
      &gsn_sleep,    SLOT(38),   15,   12,
      "",         "You feel less tired."
   }
   ,

   {
      "stone skin",     {
         25, 40, 40, 45       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_stone_skin, TAR_CHAR_SELF,    POS_STANDING,
      NULL,       SLOT(66),   12,   18,
      "",         "Your skin feels soft again."
   }
   ,

   {
      "summon",      {
         24, 12, 29, 30       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_summon,     TAR_IGNORE,    POS_STANDING,
      NULL,       SLOT(40),   50,   12,
      "",         "!Summon!"
   }
   ,

   {
      "teleport",    {
         13, 22, 25, 36       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_teleport,      TAR_CHAR_SELF,    POS_FIGHTING,
      NULL,       SLOT( 2),   35,   12,
      "",         "!Teleport!"
   }
   ,

   {
      "teleport object",   {
         60, 60, 60, 60       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_teleport_object,  TAR_CHAR_OTHER,    POS_STANDING,
      NULL,       SLOT( 205), 35,   12,
      "",         "!Teleport Object!"
   }
   ,

   {
      "undo spell",           {
         60, 60, 60, 60      }
      ,      {
         1,  1,  2,  2      }
      ,
      spell_undo_spell,       TAR_CHAR_OTHER,         POS_FIGHTING,
      NULL,                   SLOT(199),       12,     12,
      "",                     "!Undo Spell!"
   }
   ,

   {
      "ventriloquate",  {
         1, 60, 60, 60       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_ventriloquate, TAR_IGNORE,    POS_STANDING,
      NULL,       SLOT(41),    5,   12,
      "",         "!Ventriloquate!"
   }
   ,

   {
      "weaken",      {
         11, 14, 16, 17       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_weaken,     TAR_CHAR_OFFENSIVE,  POS_FIGHTING,
      NULL,       SLOT(68),   20,   12,
      "spell",    "You feel stronger."
   }
   ,

   {
      "word of recall", {
         32, 28, 32, 33       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_word_of_recall,   TAR_CHAR_SELF,    POS_RESTING,
      NULL,       SLOT(42),    5,   12,
      "",         "!Word of Recall!"
   }
   ,

   /*
    * Dragon breath
    */
   {
      "acid breath",    {
         42, 44, 46, 48       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_acid_breath,   TAR_CHAR_OFFENSIVE,  POS_FIGHTING,
      NULL,       SLOT(200),   0,    6,
      "blast of acid",  "!Acid Breath!"
   }
   ,

   {
      "fire breath",    {
         38, 40, 42, 44       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_fire_breath,   TAR_CHAR_OFFENSIVE,  POS_FIGHTING,
      NULL,       SLOT(201),   0,    6,
      "blast of flame", "!Fire Breath!"
   }
   ,

   {
      "frost breath",      {
         30, 32, 34, 36       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_frost_breath,  TAR_CHAR_OFFENSIVE,  POS_FIGHTING,
      NULL,       SLOT(202),   0,    6,
      "blast of frost", "!Frost Breath!"
   }
   ,

   {
      "gas breath",     {
         46, 48, 50, 52       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_gas_breath, TAR_IGNORE,    POS_FIGHTING,
      NULL,       SLOT(203),   0,    6,
      "blast of gas",      "!Gas Breath!"
   }
   ,

   {
      "lightning breath",  {
         34, 36, 38, 40       }
      ,     {
         1,  1,  2,  2      }
      ,
      spell_lightning_breath, TAR_CHAR_OFFENSIVE,  POS_FIGHTING,
      NULL,       SLOT(204),   0,    6,
      "blast of lightning",   "!Lightning Breath!"
   }
   ,

   /*
    * Spells for mega1.are from Glop/Erkenbrand.
    */
   {
      "general purpose",      {
         53, 53, 53, 53       }
      ,  {
         0, 0, 0, 0       }
      ,
      spell_general_purpose,  TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
      NULL,                   SLOT(401),      0,      12,
      "general purpose ammo", "!General Purpose Ammo!"
   }
   ,

   {
      "high explosive",       {
         53, 53, 53, 53       }
      ,  {
         0, 0, 0, 0       }
      ,
      spell_high_explosive,   TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
      NULL,                   SLOT(402),      0,      12,
      "high explosive ammo",  "!High Explosive Ammo!"
   }
   ,


   /* combat and weapons skills */


   {
      "axe",         {
         1, 1, 1,  1       }
      ,  {
         6, 6, 4, 2      }
      ,
      spell_null,             TAR_IGNORE,             POS_FIGHTING,
      &gsn_axe,               SLOT( 0),       0,      0,
      "",                     "!Axe!"
   }
   ,

   {
      "dagger",               {
         1,  1,  1,  1       }
      ,     {
         2, 3, 2, 3      }
      ,
      spell_null,             TAR_IGNORE,             POS_FIGHTING,
      &gsn_dagger,            SLOT( 0),       0,      0,
      "",                     "!Dagger!"
   }
   ,

   {
      "flail",    {
         1,  1, 1,  1       }
      ,  {
         6, 3, 4, 5      }
      ,
      spell_null,             TAR_IGNORE,             POS_FIGHTING,
      &gsn_flail,             SLOT( 0),       0,      0,
      "",                     "!Flail!"
   }
   ,

   {
      "mace",        {
         1,  1,  1,  1       }
      ,  {
         5, 2, 3, 3      }
      ,
      spell_null,             TAR_IGNORE,             POS_FIGHTING,
      &gsn_mace,              SLOT( 0),       0,      0,
      "",                     "!Mace!"
   }
   ,

   {
      "polearm",     {
         1, 1, 1,  1       }
      ,  {
         6, 6, 4, 6      }
      ,
      spell_null,             TAR_IGNORE,             POS_FIGHTING,
      &gsn_polearm,           SLOT( 0),       0,      0,
      "",                     "!Polearm!"
   }
   ,

   {
      "shield block",      {
         1,  1, 1,  1       }
      ,  {
         6, 4, 3, 2      }
      ,
      spell_null,    TAR_IGNORE,    POS_FIGHTING,
      &gsn_shield_block,   SLOT(0), 0, 0,
      "",         "!Shield!"
   }
   ,

   {
      "spear",    {
         1,  1,  1,  1       }
      ,  {
         4, 4, 4, 3      }
      ,
      spell_null,             TAR_IGNORE,             POS_FIGHTING,
      &gsn_spear,             SLOT( 0),       0,      0,
      "",                     "!Spear!"
   }
   ,

   {
      "sword",    {
         1, 1,  1,  1      }
      ,  {
         8, 6, 2, 4      }
      ,
      spell_null,             TAR_IGNORE,             POS_FIGHTING,
      &gsn_sword,             SLOT( 0),       0,      0,
      "",                     "!sword!"
   }
   ,

   {
      "whip",        {
         1, 1,  1,  1      }
      ,  {
         6, 5, 5, 4      }
      ,
      spell_null,             TAR_IGNORE,             POS_FIGHTING,
      &gsn_whip,              SLOT( 0),       0,      0,
      "",                     "!Whip!"
   }
   ,

   {
      "backstab",             {
         30, 30,  30, 30       }
      ,     {
   SKILL_ASSASSIN, SKILL_ASSASSIN, SKILL_ASSASSIN, SKILL_ASSASSIN  }
      ,
      spell_null,             TAR_IGNORE,             POS_STANDING,
      &gsn_backstab,          SLOT( 0),        0,     24,
      "backstab",             "!Backstab!"
   }
   ,

   {
      "bash",        {
         60, 60, 5,  1       }
      ,  {
         SKILL_ATTAINABLE, SKILL_ATTAINABLE, 3, 2      }
      ,
      spell_null,             TAR_IGNORE,             POS_FIGHTING,
      &gsn_bash,              SLOT( 0),       0,      24,
      "bash",                 "!Bash!"
   }
   ,

   {
      "berserk",     {
         60, 60, 25, 15       }
      ,  {
         SKILL_ATTAINABLE, SKILL_ATTAINABLE, 6, 4      }
      ,
      spell_null,             TAR_IGNORE,             POS_FIGHTING,
      &gsn_berserk,           SLOT( 0),       0,      24,
      "",                     "You feel your pulse slow down."
   }
   ,

   {
      "dirt kicking",      {
         60, 60,  6,  3       }
      ,  {
         SKILL_ATTAINABLE, SKILL_ATTAINABLE, 5, 4      }
      ,
      spell_null,    TAR_IGNORE,    POS_FIGHTING,
      &gsn_dirt,     SLOT( 0),   0, 24,
      "kicked dirt",    "You rub the dirt out of your eyes."
   }
   ,

   {
      "disarm",               {
         60, 60, 6, 11       }
      ,     {
         SKILL_ATTAINABLE, SKILL_ATTAINABLE, 4, 6      }
      ,
      spell_null,             TAR_IGNORE,             POS_FIGHTING,
      &gsn_disarm,            SLOT( 0),        0,     24,
      "",                     "!Disarm!"
   }
   ,

   {
      "dodge",                {
         20, 22,  5, 1       }
      ,     {
         8, 8, 8, 6      }
      ,
      spell_null,             TAR_IGNORE,             POS_FIGHTING,
      &gsn_dodge,             SLOT( 0),        0,      0,
      "",                     "!Dodge!"
   }
   ,

   {
      "enhanced damage",      {
         60, 60, 5,  1       }
      ,     {
         SKILL_ATTAINABLE, 9, 5, 3      }
      ,
      spell_null,             TAR_IGNORE,             POS_FIGHTING,
      &gsn_enhanced_damage,   SLOT( 0),        0,      0,
      "",                     "!Enhanced Damage!"
   }
   ,

   {
      "hand to hand",      {
         25,  10, 10, 6       }
      ,  {
         8, 5, 4, 3      }
      ,
      spell_null,    TAR_IGNORE,    POS_FIGHTING,
      &gsn_hand_to_hand,   SLOT( 0),   0, 0,
      "",         "!Hand to Hand!"
   }
   ,

   {
      "kick",                 {
         60, 12, 12,  8       }
      ,     {
         SKILL_ATTAINABLE, 4, 4, 3      }
      ,
      spell_null,             TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
      &gsn_kick,              SLOT( 0),        0,      12,
      "kick",                 "!Kick!"
   }
   ,

   {
      "headbutt",             {
         60, 60, 20, 5       }
      ,      {
         SKILL_ATTAINABLE, SKILL_ATTAINABLE, 8, 3      }
      ,
      spell_null,             TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
      &gsn_headbutt,          SLOT( 0),        0,      12,
      "headbutt",             "You wipe the blood from your eyes."
   }
   ,

   {
      "parry",                {
         22, 20, 1,  5       }
      ,     {
         8, 8, 4, 5      }
      ,
      spell_null,             TAR_IGNORE,             POS_FIGHTING,
      &gsn_parry,             SLOT( 0),        0,      0,
      "",                     "!Parry!"
   }
   ,

   {
      "rescue",               {
         60, 60, 3,  1       }
      ,     {
         SKILL_ATTAINABLE, 6, 2, 4      }
      ,
      spell_null,             TAR_IGNORE,             POS_FIGHTING,
      &gsn_rescue,            SLOT( 0),        0,     12,
      "",                     "!Rescue!"
   }
   ,

   {
      "trip",        {
         60, 60,  1, 1       }
      ,  {
         SKILL_ATTAINABLE, SKILL_ATTAINABLE, 4, 3      }
      ,
      spell_null,    TAR_IGNORE,    POS_FIGHTING,
      &gsn_trip,     SLOT( 0),   0, 24,
      "trip",        "!Trip!"
   }
   ,

   {
      "second attack",        {
         30, 24, 7,  5       }
      ,     {
         10, 8, 4, 3      }
      ,
      spell_null,             TAR_IGNORE,             POS_FIGHTING,
      &gsn_second_attack,     SLOT( 0),        0,      0,
      "",                     "!Second Attack!"
   }
   ,

   {
      "third attack",         {
         60, 60, 14, 12       }
      ,     {
         SKILL_ATTAINABLE, SKILL_ATTAINABLE, 6, 4      }
      ,
      spell_null,             TAR_IGNORE,             POS_FIGHTING,
      &gsn_third_attack,      SLOT( 0),        0,      0,
      "",                     "!Third Attack!"
   }
   ,

   /* non-combat skills */

   {
      "fast healing",      {
         15, 9, 8,  6       }
      ,  {
         8, 5, 4, 3      }
      ,
      spell_null,    TAR_IGNORE,    POS_SLEEPING,
      &gsn_fast_healing,   SLOT( 0),   0, 0,
      "",         "!Fast Healing!"
   }
   ,

   {
      "haggle",      {
         7, 18,  10, 14       }
      ,  {
         5, 8, 6, 6      }
      ,
      spell_null,    TAR_IGNORE,    POS_RESTING,
      &gsn_haggle,      SLOT( 0),   0, 0,
      "",         "!Haggle!"
   }
   ,

   {
      "hide",        {
         30, 30,  30,  30       }
      ,  {
       SKILL_ASSASSIN, SKILL_ASSASSIN, SKILL_ASSASSIN, SKILL_ASSASSIN}
      ,
      spell_null,    TAR_IGNORE,    POS_RESTING,
      &gsn_hide,     SLOT( 0),    0,   12,
      "",         "!Hide!"
   }
   ,

   {
      "lore",        {
         10, 10,  16, 20       }
      ,  {
         6, 6, 8, 8      }
      ,
      spell_null,    TAR_IGNORE,    POS_RESTING,
      &gsn_lore,     SLOT( 0),   0, 36,
      "",         "!Lore!"
   }
   ,

   {
      "meditation",     {
         6,  6, 15, 15       }
      ,  {
         5, 5, 8, 8      }
      ,
      spell_null,    TAR_IGNORE,    POS_SLEEPING,
      &gsn_meditation,  SLOT( 0),   0, 0,
      "",         "Meditation"
   }
   ,

   {
      "peek",        {
         30, 30,  30, 30       }
      ,  {
      SKILL_ASSASSIN, SKILL_ASSASSIN, SKILL_ASSASSIN, SKILL_ASSASSIN }
      ,
      spell_null,    TAR_IGNORE,    POS_STANDING,
      &gsn_peek,     SLOT( 0),    0,    0,
      "",         "!Peek!"
   }
   ,


   {
      "ride",                 {
         60, 60, 5, 60       }
      ,      {
         SKILL_ATTAINABLE, SKILL_ATTAINABLE, 4, SKILL_ATTAINABLE      }
      ,
      spell_null,             TAR_IGNORE,             POS_STANDING,
      &gsn_ride,              SLOT( 0),       0,      0,
      "",                     "!Ride!"
   }
   ,

   {
      "sharpen",              {
         60, 60, 15, 60       }
      ,     {
         SKILL_ATTAINABLE, SKILL_ATTAINABLE, 4, SKILL_ATTAINABLE  }
      ,
      spell_null,             TAR_OBJ_INV,            POS_STANDING,
      &gsn_sharpen,           SLOT( 0),        0,     0,
      "",                     "!Sharpen!"
   }
   ,

   {
      "pick lock",      {
         25, 25, 25, 25       }
      ,  {
         5, 5, 5, 5      }
      ,
      spell_null,    TAR_IGNORE,    POS_STANDING,
      &gsn_pick_lock,      SLOT( 0),    0,   12,
      "",         "!Pick!"
   }
   ,

   {
      "sneak",    {
         30, 30,  30,  30       }
      ,  {
    SKILL_ASSASSIN, SKILL_ASSASSIN, SKILL_ASSASSIN, SKILL_ASSASSIN}
      ,
      spell_null,    TAR_IGNORE,    POS_STANDING,
      &gsn_sneak,    SLOT( 0),    0,   12,
      "",         "You no longer feel stealthy."
   }
   ,

   {
      "steal",    {
         30, 30,  30, 30       }
      ,  {
    SKILL_ASSASSIN, SKILL_ASSASSIN, SKILL_ASSASSIN, SKILL_ASSASSIN}
      ,
      spell_null,    TAR_IGNORE,    POS_STANDING,
      &gsn_steal,    SLOT( 0),    0,   24,
      "",         "!Steal!"
   }
   ,

   {
      "scrolls",     {
         1,  1,  1,  1       }
      ,  {
         2, 3, 6, 8      }
      ,
      spell_null,    TAR_IGNORE,    POS_STANDING,
      &gsn_scrolls,     SLOT( 0),   0, 24,
      "",         "!Scrolls!"
   }
   ,

   {
      "staves",      {
         1,  1,  1,  1       }
      ,  {
         2, 3, 6, 8      }
      ,
      spell_null,    TAR_IGNORE,    POS_STANDING,
      &gsn_staves,      SLOT( 0),   0, 12,
      "",         "!Staves!"
   }
   ,

   {
      "wands",    {
         1,  1,  1,  1       }
      ,  {
         2, 3, 6, 8      }
      ,
      spell_null,    TAR_IGNORE,    POS_STANDING,
      &gsn_wands,    SLOT( 0),   0, 12,
      "",         "!Wands!"
   }
   ,

   {
      "throw",                {
         15, 15, 15, 15       }
      ,     {
         2, 2, 3, 5      }
      ,
      spell_null,             TAR_IGNORE,             POS_STANDING,
      &gsn_throw,             SLOT( 0),       0,      12,
      "",                     "!Throw!"
   }
   ,

   {
      "recall",      {
         1,  1,  1,  1       }
      ,  {
         2, 2, 2, 2      }
      ,
      spell_null,    TAR_IGNORE,    POS_STANDING,
      &gsn_recall,      SLOT( 0),   0, 12,
      "",         "!Recall!"
   },

   /* commented out for time being --Fara
     { 
      "raise dead",      {
         1,  1,  1,  1       }
      ,  {
	SKILL_ATTAINABLE, SKILL_ATTAINABLE,
	SKILL_ATTAINABLE, SKILL_ATTAINABLE }
      ,
      spell_raise_dead,    TAR_IGNORE,    POS_STANDING,
      &gsn_raise_dead,      SLOT( 209),   100, 12,
      "",         "!Raise dead!"
      }, */

};

const   struct  group_type      group_table     [MAX_GROUP]     =
{

   {
      "rom basics",     {
         0, 0, 0, 0       }
      ,
      {
         "scrolls", "staves", "wands", "recall", "throw"       }
   }
   ,

   {
      "mage basics",    {
         0, -3, -3, -3       }
      ,
      {
         "dagger"       }
   }
   ,

   {
      "cleric basics",  {
         -3, 0, -3, -3       }
      ,
      {
         "mace"       }
   }
   ,

   {
      "knight basics",  {
         -3, -3, 0, -3       }
      ,
      {
         "sword", "ride"       }
   }
   ,

   {
      "barbarian basics",  {
         -3, -3, -3, 0       }
      ,
      {
         "axe", "enhanced damage"       }
   }
   ,

   {
      "mage default",      {
         40, -3, -3, -3       }
      ,
      {
         "lore", "beguiling", "combat", "detection",
         "enhancement", "illusion", "maladictions", "protective",
         "transportation", "weather"       }
   }
   ,

   {
      "cleric default", {
         -3, 40, -3, -3       }
      ,
      {
         "flail", "attack", "creation", "curative",  "benedictions",
         "detection", "healing", "maladictions", "protective",
         "shield block", "transportation", "weather"       }
   }
   ,

   {
      "knight default", {
         -3, -3, 40, -3       }
      ,
      {
         "second attack", "shield block",
         "parry", "rescue", "disarm", "weaponsmaster", "bash"       }
   }
   ,

   {
      "barbarian default", {
         -3, -3, -3, 40       }
      ,
      {
         "shield block", "dodge", "berserk", "dirt kick",
         "second attack", "third attack", "kick", "hand to hand",
         "trip", "weapon craft"       }
   }
   ,

   {
      "assassin",             {
         20, 20, 20, 20       }
      ,
      {
         "sneak", "hide", "backstab", "steal", "peek", "dagger"       }
   }
   ,

   {
      "weaponsmaster",  {
         40, 35, 20, 25       }
      ,
      {
         "axe", "dagger", "flail", "mace", "polearm", "spear", "sword", "whip"       }
   }
   ,

   {
      "attack",      {
         -1, 5, 8, -1       }
      ,
      {
         "demonfire", "dispel evil", "dispel good", "earthquake",
         "flamestrike", "exorcise"       }
   }
   ,

   {
      "weapon craft",          {
         -1, -1, -1, 8       }
      ,
      {
         "venom", "tame lightning", "black death", "acid wash",
         "damnation"       }
   }
   ,

   {
      "beguiling",      {
         4, -1, -1, -1       }
      ,
      {
         "charm person", "sleep"       }
   }
   ,

   {
      "benedictions",      {
         -1, 4, 8, -1       }
      ,
      {
         "bless", "calm", "frenzy", "holy word", "remove curse",
         "remove invisible", "remove alignment"       }
   }
   ,

   {
      "combat",      {
         6, -1, -1, -1       }
      ,
      {
         "acid blast", "burning hands", "chain lightning", "chill touch",
         "colour spray", "fireball", "lightning bolt", "magic missile",
         "shocking grasp"       }
   }
   ,

   {
      "creation",    {
         4, 4, 8, 8       }
      ,
      {
         "continual light", "create food", "create spring", "create water"       }
   }
   ,

   {
      "curative",    {
         -1, 4, 6, 8       }
      ,
      {
         "cure blindness", "cure disease", "cure poison"       }
   }
   ,

   {
      "detection",      {
         4, 3, -1, -1       }
      ,
      {
         "detect evil", "detect hidden", "detect invis", "detect magic",
         "detect poison", "identify", "know alignment", "locate object"       }
   }
   ,

   {
      "draconian",      {
         8, -1, -1, -1       }
      ,
      {
         "acid breath", "fire breath", "frost breath", "gas breath",
         "lightning breath"        }
   }
   ,

   {
      "enchantment",    {
         6, -1, -1, -1       }
      ,
      {
         "enchant armor", "enchant weapon"       }
   }
   ,

   {
      "enhancement",    {
         5, -1, 9, 10       }
      ,
      {
         "giant strength", "haste", "infravision",
         "refresh", "regeneration"       }
   }
   ,

   {
      "harmful",     {
         -1, 3, 6, -1       }
      ,
      {
         "cause critical", "cause light", "cause serious", "harm"       }
   }
   ,

   {
      "healing",     {
         -1, 3, 6, -1       }
      ,
      {
         "cure critical", "cure light", "cure serious", "heal",
         "mass healing", "refresh"       }
   }
   ,

   {
      "illusion",    {
         4, -1, -1, -1       }
      ,
      {
         "invis", "mass invis", "ventriloquate"       }
   }
   ,

   {
      "maladictions",      {
         5, 5, 8, 10       }
      ,
      {
         "blindness", "change sex", "curse", "energy drain", "plague",
         "poison", "weaken", "lethargy", "insanity"       }
   }
   ,

   {
      "protective",     {
         4, 4, 8, 10       }
      ,
      {
         "armor", "cancellation", "dispel magic", "protection evil",
         "sanctuary", "shield", "stone skin", "protection good"       }
   }
   ,

   {
      "specialities",         {
         10, 10, 10, 10      }
      ,
      {
         "teleport object", "undo spell", "protect container", "talon"       }
   }
   ,

   {
      "transportation", {
         4, 4, 8, 10       }
      ,
      {
         "fly", "gate", "pass door", "summon", "teleport", "word of recall",
         "portal"       }
   }
   ,

   {
      "weather",     {
         4, 4, 8, 8       }
      ,
      {
         "call lightning", "control weather", "faerie fire", "faerie fog",
         "lightning bolt", "octarine fire", "psychic tornado" }
   }



};

char deity_name_area[256] = "The Moog";
char *deity_name = deity_name_area;

const struct materials_type material_table[] =
{
   /* { percentage resilience, name } */
   {
      0, "none"    }
   ,
   {
      40, "default"    }
   ,
   {
      90, "adamantite"    }
   ,
   {
      70, "iron"    }
   ,
   {
      15, "glass"    }
   ,
   {
      71, "bronze"    }
   ,
   {
      30, "cloth"    }
   ,
   {
      35, "wood"    }
   ,
   {
      10, "paper"    }
   ,
   {
      75, "steel"    }
   ,
   {
      85, "stone"    }
   ,
   {
      0, "food"    }
   ,
   {
      55, "silver"    }
   ,
   {
      55, "gold"    }
   ,
   {
      30, "leather"    }
   ,
   {
      20, "vellum"    }
   ,
   {
      5, "china"    }
   ,
   {
      10, "clay"    }
   ,
   {
      75, "brass"    }
   ,
   {
      45, "bone"    }
   ,
   {
      82, "platinum"    }
   ,
   {
      40, "pearl"    }
   ,
   {
      65, "mithril"    }
   ,
   {
      100, "octarine"    }
   ,
   {
      0, NULL    }
};



/*
 * Extra flag names
 */

const char *flagname_extra[64] = {
	"wnet ",       /* 0 */
	"wn_debug ",
	"wn_mort ",
	"wn_imm ",
	"wn_bug ",
	"permit ",    /* 5 */
	"wn_tick",
	"",
	"",
	"info_name ",
	"info_email ",           /* 10 */
	"info_mes ",
	"info_url ",
	"",
	"tip_std ",
	"tip_olc",          /*15 */ 
	"tip_adv",
	"",
	"",
	"",
	"",           /*20*/
	"",
	"",
	"",
	"",
	"",        /* 25 */
	"",
	"",
	"",
	"",
	"",           /* 30 */
	"",
	"",
	"",
	"",
	"",           /* 35 */
	"",
	"",
	"",
	"",
	"",           /* 40 */
	"",
	"",
	"",
	"",
	"",           /* 45 */
	"",
	"",
	"",
	"",
	"",           /* 50 */
	"",
	"",
	"",
	"",
	"",           /*55*/
	"",
	"",
	"",
	"",
	"",           /*60*/
	"",
	"",
	""              /* 64 */
};

