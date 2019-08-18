#include "util/memory_usage.h"

#include <catch.hpp>

TEST_CASE(
    "getMemoryUsage",
    "[small][util]")
{
    constexpr std::size_t MEGA_BYTES = 1024 * 1042;

    std::optional<std::size_t> first = util::getMemoryUsage();
    REQUIRE(first.has_value());

    std::vector<char> buffer(MEGA_BYTES);

    std::optional<std::size_t> second = util::getMemoryUsage();
    REQUIRE(second.has_value());

    CHECK(MEGA_BYTES <= (*second - *first));
}
