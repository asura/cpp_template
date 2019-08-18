#include "util/Logger.h"

#include "util/memory_usage.h"  // getMemoryUsage

namespace util
{
void Logger::args(
    std::ostream& the_ostream)
{
}

/// @brief spdlog出力
template <typename... Args>
void logging(
    Logger::Level the_level,
    const char* the_format,
    Args... the_args);

void Logger::pre(
    const std::string& the_file_name,
    int the_line_number,
    const std::string& the_function_name)
{
    preImpl(the_file_name, the_line_number, the_function_name, "start");
}

void Logger::preImpl(
    const std::string& the_file_name,
    int the_line_number,
    const std::string& the_function_name,
    const std::string& the_message)
{
    const auto rss = util::getMemoryUsage();
    if (!rss.has_value())
    {
        m_memory_usage_at_start = 0;
        return;
    }

    m_memory_usage_at_start = *rss;

    logging(
        Level::INF,
        "{}:{} {} -- {}, rss={}",
        the_file_name,
        the_line_number,
        the_function_name,
        the_message,
        m_memory_usage_at_start);
}

void Logger::post(
    Level the_level,
    const std::string& the_file_name,
    int the_line_number,
    const std::string& the_function_name)
{
    postImpl(the_level, the_file_name, the_line_number, the_function_name, "end");
}

void Logger::postImpl(
    Level the_level,
    const std::string& the_file_name,
    int the_line_number,
    const std::string& the_function_name,
    const std::string& the_message)
{
    const auto rss = util::getMemoryUsage();
    if (!rss.has_value())
    {
        return;
    }

    const std::size_t memory_usage_at_end = *rss;

    logging(
        the_level,
        "{}:{} {} -- {}, rss={}(+{})",
        the_file_name,
        the_line_number,
        the_function_name,
        the_message,
        memory_usage_at_end,
        memory_usage_at_end - m_memory_usage_at_start);
}

Logger& theLogger()
{
    static Logger the_logger;
    return the_logger;
}
}  // namespace util

#include "Logger_spdlog.h"
