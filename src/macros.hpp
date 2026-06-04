#pragma once

#include "config.hpp"

#ifdef DEBUG_LOGS
    #define errorf(frmt,...) printf("\e[31mERROR: " frmt "\e[0m\n", ##__VA_ARGS__)
#else
    #define errorf(frmt,...)
#endif