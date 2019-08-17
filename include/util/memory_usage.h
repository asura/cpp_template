#ifndef UTIL_MEMORY_USAGE_H
#define UTIL_MEMORY_USAGE_H

#include <optional>

namespace util
{
/// プロセスが現在使っているメモリ量を取得する
std::optional<std::size_t> getMemoryUsage();
}  // namespace util

#endif  // UTIL_MEMORY_USAGE_H
