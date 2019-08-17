#include "util/Logger.h"

#include "util/memory_usage.h"  // getMemoryUsage

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/syslog_sink.h>
#include <spdlog/spdlog.h>

namespace util
{
Logger::LoggersRegister::LoggersRegister(
    const std::string& the_category_name)
{
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
#ifndef NDEBUG
    console_sink->set_level(spdlog::level::debug);
#else
    console_sink->set_level(spdlog::level::info);
#endif
    // ログレベル(1文字)、日時、メッセージ
    console_sink->set_pattern("%L, [%Y-%m-%d %H:%M:%S.%f] %^%v%$");

    auto syslog_sink = std::make_shared<spdlog::sinks::syslog_sink_mt>(the_category_name);
    syslog_sink->set_level(spdlog::level::err);

    spdlog::set_default_logger(
        std::make_shared<spdlog::logger>(
            "multi_sink",
            spdlog::sinks_init_list(
                {console_sink,
                 syslog_sink})));
}

void Logger::args(
    std::ostream& the_ostream)
{
}

void Logger::pre(
    const std::string& the_file_name,
    int the_line_number,
    const std::string& the_function_name)
{
    pre_impl(the_file_name, the_line_number, the_function_name, "start");
}

void Logger::pre_impl(
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

    spdlog::info(
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
    post_impl(the_level, the_file_name, the_line_number, the_function_name, "end");
}

void Logger::post_impl(
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

    switch (the_level)
    {
    case Level::INF:
        spdlog::info(
            "{}:{} {} -- {}, rss={}(+{})",
            the_file_name,
            the_line_number,
            the_function_name,
            the_message,
            *rss,
            *rss - m_memory_usage_at_start);
        break;
    case Level::WRN:
        spdlog::warn(
            "{}:{} {} -- {}, rss={}/{}",
            the_file_name,
            the_line_number,
            the_function_name,
            the_message,
            *rss,
            *rss - m_memory_usage_at_start);
        break;
    case Level::ERR:
        spdlog::error(
            "{}:{} {} -- {}, rss={}/{}",
            the_file_name,
            the_line_number,
            the_function_name,
            the_message,
            *rss,
            *rss - m_memory_usage_at_start);
        break;
    }
}

Logger& theLogger()
{
    static Logger the_logger;
    return the_logger;
}
}  // namespace util
