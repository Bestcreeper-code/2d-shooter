#pragma once

#include "config.hpp"

#ifdef DEBUG_BUILD
#define errorf(frmt,...) \
    printf("(" __FILE_NAME__ ":" __LINE__ ")\e[31mERROR: " frmt "\e[0m\n", ##__VA_ARGS__)
#else
    #define errorf(frmt,...)
#endif

#ifdef VERBOSE_ERRORS
#define verbose_errf(frmt, ...) \
    printf("\e[31m(%s:%d) " frmt "\e[0m\n", __FILE_NAME__, __LINE__, ##__VA_ARGS__)
#else
    #define verbose_errf(frmt,...)
#endif



#define DARKERGRAY   CLITERAL(Color){ 60, 60, 60, 255 }