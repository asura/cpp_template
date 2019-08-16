#define CATCH_CONFIG_RUNNER
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <catch.hpp>

int main(int argc, char** argv)
{
    try
    {
        auto logger = spdlog::stdout_color_mt("Tester");
        logger->info("START");
        const int result = Catch::Session().run(argc, argv);
        logger->info("END (result={})", result);
        return result;
    }
    catch (...)
    {
        return -1;
    }
}
