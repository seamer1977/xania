add_library(xania_common STATIC
        Fd.cpp Fd.hpp
        doorman_protocol.h
        mask_hostname.hpp mask_hostname.cpp)
target_link_libraries(xania_common CONAN_PKG::fmt CONAN_PKG::ms-gsl CONAN_PKG::spdlog)
target_include_directories(xania_common PUBLIC ..)

add_subdirectory(test)
