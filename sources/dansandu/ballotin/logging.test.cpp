#include "dansandu/ballotin/logging.hpp"
#include "catchorg/catch2/catch_test_macros.hpp"

#include <stdexcept>
#include <string>
#include <string_view>

using dansandu::ballotin::logging::Level;
using dansandu::ballotin::logging::LogEntry;
using dansandu::ballotin::logging::Logger;

TEST_CASE("logging")
{
    auto logger = Logger{};

    SECTION("level matching")
    {
        const auto expectedFunction = "function";
        const auto expectedFile = "file";
        const auto expectedLine = 17;
        const auto expectedLevel = Level::debug;
        const auto expectedMessage = "message";

        auto logged = false;

        logger.addHandler("test", Level::debug,
                          [&](const LogEntry& logEntry)
                          {
                              REQUIRE(logEntry.function == expectedFunction);

                              REQUIRE(logEntry.file == expectedFile);

                              REQUIRE(logEntry.line == expectedLine);

                              REQUIRE(logEntry.level == expectedLevel);

                              REQUIRE(logEntry.message == expectedMessage);

                              logged = true;
                          });

        logger.log(expectedLevel, expectedFunction, expectedFile, expectedLine, expectedMessage);

        REQUIRE(logged);
    }

    SECTION("level not matching")
    {
        auto logged = false;

        logger.addHandler("test", Level::info, [&](const LogEntry&) { logged = true; });

        logger.log(Level::debug, "function", "file", 3, "message");

        REQUIRE(!logged);
    }

    SECTION("duplicate handler")
    {
        const auto name = "default";
        const auto level = Level::debug;
        const auto handler = [&](const LogEntry&) {};

        logger.addHandler(name, level, handler);

        REQUIRE_THROWS_AS(logger.addHandler(name, level, handler), std::logic_error);
    }

    SECTION("remove handler")
    {
        auto logged = false;

        logger.addHandler("test", Level::error, [&](const LogEntry&) { logged = true; });

        logger.removeHandler("test");

        logger.log(Level::error, "function", "file", 3, "message");

        REQUIRE(!logged);
    }
}
