#include "osdep.hpp"

#include <fstream>
#include <sys/sysinfo.h>
#include <unistd.h>



size_t GetTotalSystemMem()
{
    struct sysinfo info;
    sysinfo(&info);
    return (size_t)info.totalram * info.mem_unit;
}

size_t GetProcessMemUsage()
{
    std::ifstream file("/proc/self/statm");
    size_t size = 0, resident = 0;
    file >> size >> resident;

    long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024;
    return resident * page_size_kb * 1024;
}