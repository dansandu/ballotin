#include "dansandu/ballotin/logging.hpp"
#include "catchorg/catch/catch.hpp"

#include <stdexcept>
#include <string>
#include <string_view>

using dansandu::ballotin::logging::Level;
using dansandu::ballotin::logging::LogEntry;
using dansandu::ballotin::logging::Logger;

TEST_CASE("logging")
{
    SECTION("level comparison")
    {
        STATIC_REQUIRE(Level::none < Level::error);

        STATIC_REQUIRE(Level::error < Level::warn);

        STATIC_REQUIRE(Level::warn < Level::info);

        STATIC_REQUIRE(Level::info < Level::debug);

        STATIC_REQUIRE(Level::debug == Level::debug);

        STATIC_REQUIRE(Level::info != Level::debug);

        STATIC_REQUIRE(Level::debug >= Level::info);
    }

    auto logger = Logger{};

    SECTION("level matching")
    {
        const auto expectedFunction = "function";
        const auto expectedFile = "file";
        const auto expectedLine = 17;
        const auto expectedLevel = Level::debug;
        const auto expectedMessage = L"message";

        auto logged = false;

        logger.addHandler(L"test", Level::debug,
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

        logger.addHandler(L"test", Level::info, [&](const LogEntry&) { logged = true; });

        logger.log(Level::debug, "function", "file", 3, L"message");

        REQUIRE(!logged);
    }

    SECTION("duplicate handler")
    {
        const auto name = L"default";
        const auto level = Level::debug;
        const auto handler = [&](const LogEntry&) {};

        logger.addHandler(name, level, handler);

        REQUIRE_THROWS_AS(logger.addHandler(name, level, handler), std::logic_error);
    }

    SECTION("remove handler")
    {
        auto logged = false;

        const auto name = L"test";

        logger.addHandler(name, Level::error, [&](const LogEntry&) { logged = true; });

        logger.removeHandler(name);

        logger.log(Level::error, "function", "file", 3, L"message");

        REQUIRE(!logged);
    }
}
