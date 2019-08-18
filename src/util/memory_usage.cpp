#include "util/memory_usage.h"

#if defined(__linux__) || defined(__linux) || defined(linux) || defined(__gnu_linux__)
#include <unistd.h>  // sysconf

#include <fstream>
#include <iostream>
#elif defined(__APPLE__) && defined(__MACH__)
#include <mach/kern_return.h>  // kern_return_t
#include <mach/mach_init.h>    // mach_task_self
#include <mach/message.h>      // mach_msg_type_number_t
#include <mach/task.h>         // task_info
#include <mach/task_info.h>    // MACH_TASK_BASIC_INFO, MACH_TASK_BASIC_INFO_COUNT, mach_task_basic_info_data_t, task_info_t
#elif defined(_WIN32)
#include <psapi.h>
#include <windows.h>
#endif

namespace util
{
std::optional<std::size_t> getMemoryUsage()
{
#if defined(__linux__) || defined(__linux) || defined(linux) || defined(__gnu_linux__)
    std::ifstream stat_stream("/proc/self/stat");
    if (stat_stream.fail())
    {
        std::cerr << "file open failed" << std::endl;
        return std::nullopt;
    }

    std::string pid, comm, state, ppid, pgrp, session, tty_nr;
    std::string tpgid, flags, minflt, cminflt, majflt, cmajflt;
    std::string utime, stime, cutime, cstime, priority, nice;
    std::string o, itrealvalue, starttime;
    uint64_t vsize;
    int64_t rss;

    stat_stream >> pid >> comm >> state >> ppid >> pgrp >> session >> tty_nr;
    stat_stream >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt;
    stat_stream >> utime >> stime >> cutime >> cstime >> priority >> nice >> o >> itrealvalue >> starttime;
    stat_stream >> vsize >> rss;

    const int64_t page_size = sysconf(_SC_PAGE_SIZE);

    return rss * page_size;
#elif defined(__APPLE__) && defined(__MACH__)
    mach_task_basic_info_data_t info{};
    mach_msg_type_number_t info_count = MACH_TASK_BASIC_INFO_COUNT;
    auto result = task_info(mach_task_self(),
                            MACH_TASK_BASIC_INFO,
                            reinterpret_cast<task_info_t>(&info),
                            &info_count);
    if (result != KERN_SUCCESS)
    {
        return std::nullopt;
    }
    return static_cast<std::size_t>(info.resident_size);
#elif defined(_WIN32)
    PROCESS_MEMORY_COUNTERS info;
    GetProcessMemoryInfo(GetCurrentProcess(), &info, sizeof(info));
    return (size_t)info.WorkingSetSize;
#else
    return std::nullopt;
#endif
}
}  // namespace util
