#include "osdep.hpp"

#include "config.hpp"
#define WIN32_LEAN_AND_MEAN
#define NOGDI        // removes Rectangle()
#define NOUSER
#include <windows.h>
#include <psapi.h>

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

size_t GetAllocMemUsage(){
    return 0; 
}

size_t GetTotalSystemRAM()
{
    MEMORYSTATUSEX status;
    status.dwLength = sizeof(status);
    GlobalMemoryStatusEx(&status);
    return (size_t)status.ullTotalPhys; 
}

size_t GetTotalProcessMemUsage()
{
    PROCESS_MEMORY_COUNTERS_EX pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(),
                             (PROCESS_MEMORY_COUNTERS*)&pmc,
                             sizeof(pmc)))
    {
        return (size_t)pmc.WorkingSetSize; 
        
    }
    return 0;
}