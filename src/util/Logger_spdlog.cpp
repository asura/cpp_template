#include "util/Logger.h"

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
}  // namespace util
