#pragma once

#include "config.hpp"

#ifdef DEBUG_BUILD
    #define errorf(frmt,...) printf("\e[31mERROR: " frmt "\e[0m\n", ##__VA_ARGS__)
#else
    #define errorf(frmt,...)
#endif

#ifdef VERBOSE_ERRORS
    #define verbose_errf(frmt,...) printf("\e[31m" frmt "\e[0m\n", ##__VA_ARGS__)
#else
    #define verbose_errf(frmt,...)
#endif