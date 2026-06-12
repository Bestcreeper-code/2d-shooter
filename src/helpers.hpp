#pragma once

#include "config.hpp"
#include <string>
#include <unordered_map>

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


template <typename K, typename V>
V strmap_get_or_default(const std::unordered_map<std::string, V>& m, const K& key, const V& def) {
    auto it = m.find(key);
    return (it != m.end()) ? it->second : def;
}



#define DARKERGRAY   CLITERAL(Color){ 60, 60, 60, 255 }