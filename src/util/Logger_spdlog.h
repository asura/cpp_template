#include "util/Logger.h"

#include <spdlog/spdlog.h>

namespace util
{
/// @note spdlogへの依存は本メソッドのみ。他のLoggerクラスメソッドはspdlogとは無関係。
template <typename... Args>
void logging(
    Logger::Level the_level,
    const char* the_format,
    Args... the_args)
{
    switch (the_level)
    {
    case Logger::Level::INF:
        spdlog::info(the_format, std::forward<Args>(the_args)...);
        break;
    case Logger::Level::WRN:
        spdlog::warn(the_format, std::forward<Args>(the_args)...);
        break;
    case Logger::Level::ERR:
        spdlog::error(the_format, std::forward<Args>(the_args)...);
        break;
    }
}
}  // namespace util
