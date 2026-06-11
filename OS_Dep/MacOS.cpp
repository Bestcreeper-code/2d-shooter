#include "osdep.hpp"
#include "config.hpp"

#include <fstream>
#include <atomic>
#include <cstdlib>

#ifdef __APPLE__
#include <mach/mach.h>
#include <unistd.h>
#endif

static std::atomic<size_t> g_Allocs{0};

#ifdef DEBUG_BUILD

void* operator new(size_t size) {
    g_Allocs += size;
    return malloc(size);
}

void operator delete(void* ptr, size_t size) noexcept {
    g_Allocs -= size;
    free(ptr);
}

#endif

size_t GetAllocMemUsage() {
    return g_Allocs;
}

size_t GetTotalSystemMem()
{
#ifdef __APPLE__
    int mib[2];
    int64_t mem;
    size_t len = sizeof(mem);

    mib[0] = CTL_HW;
    mib[1] = HW_MEMSIZE;

    sysctl(mib, 2, &mem, &len, nullptr, 0);
    return static_cast<size_t>(mem);
#else
    return -1;
#endif
}

size_t GetTotalProcessMemUsage()
{
#ifdef __APPLE__
    struct mach_task_basic_info info;
    mach_msg_type_number_t count = MACH_TASK_BASIC_INFO_COUNT;

    if (task_info(mach_task_self(),
                  MACH_TASK_BASIC_INFO,
                  (task_info_t)&info,
                  &count) == KERN_SUCCESS)
    {
        return static_cast<size_t>(info.resident_size);
    }

    return -1;
#else
    return -1;
#endif
}