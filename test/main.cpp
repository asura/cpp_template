#define CATCH_CONFIG_RUNNER
#include "util/Logger.h"

#include <catch.hpp>
#include <string>
#include <vector>

int main(int the_argc, char** the_argv) noexcept
{
    try
    {
        util::Logger::LoggersRegister("Tester");

        if (1 < the_argc)
        {
            std::string args;
            for (int i = 1; i < the_argc; ++i)
            {
                if (i != 1)
                {
                    args += ' ';
                }
                args += the_argv[i];
            }

            UTIL_LOGGING_PRE(args);
        }
        else
        {
            UTIL_LOGGING_PRE0();
        }

        const int result = Catch::Session().run(the_argc, the_argv);

        UTIL_LOGGING_POST(
            ((result == 0) ? util::Logger::Level::INF : util::Logger::Level::ERR),
            result);

        return result;
    }
    catch (...)
    {
        return -1;
    }
}
