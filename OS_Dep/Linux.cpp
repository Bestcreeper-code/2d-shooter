#include "osdep.hpp"

#include "config.hpp"

#include <fstream>
#include <sys/sysinfo.h>
#include <unistd.h>

#include <atomic>
static std::atomic<size_t> g_myAllocs{0};
#ifdef DEBUG_BUILD

void* operator new(size_t size) {
    g_myAllocs += size;
    return malloc(size);
}
void operator delete(void* ptr, size_t size) noexcept {
    g_myAllocs -= size;
    free(ptr);
}

#endif
size_t GetAllocMemUsage() { return g_myAllocs; }

size_t GetTotalSystemMem()
{
    struct sysinfo info;
    sysinfo(&info);
    return (size_t)info.totalram * info.mem_unit;
}

size_t GetTotalProcessMemUsage()
{
    static std::ifstream file("/proc/self/statm");
    static long page_size = sysconf(_SC_PAGE_SIZE);
    
    file.seekg(0);
    size_t size = 0, resident = 0;
    file >> size >> resident;
    
    return resident * page_size;
}// in a debug header somewhere
