#include "dansandu/ballotin/logging.hpp"
#include "catchorg/catch/catch.hpp"

#include <stdexcept>
#include <string>
#include <string_view>

using dansandu::ballotin::logging::Level;
using dansandu::ballotin::logging::LogEntry;
using dansandu::ballotin::logging::Logger;

static void testLog(Logger& logger, const Level expectedLevel, const std::wstring_view expectedMessage,
                    std::function<void(const LogEntry&)> handler)
{
    logger.addHandler(L"test", Level::debug, std::move(handler));

    logger.log(expectedLevel, expectedMessage);
}

TEST_CASE("logging")
{
    SECTION("level comparison")
    {
        STATIC_REQUIRE(Level::none < Level::critical);

        STATIC_REQUIRE(Level::critical < Level::error);

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
        const auto expectedLine = 17;
        const auto expectedColumn = 5;
        const auto expectedLevel = Level::debug;
        const auto expectedMessage = L"message";

        auto logged = false;

        testLog(logger, expectedLevel, expectedMessage,
                [&](const LogEntry& logEntry)
                {
                    CHECK(logEntry.line == expectedLine);

                    CHECK(logEntry.column == expectedColumn);

                    CHECK(logEntry.level == expectedLevel);

                    CHECK(logEntry.message == expectedMessage);

                    logged = true;
                });

        REQUIRE(logged);
    }

    SECTION("level not matching")
    {
        auto logged = false;

        logger.addHandler(L"test", Level::info, [&](const LogEntry&) { logged = true; });

        logger.log(Level::debug, L"message");

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

        logger.log(Level::error, L"message");

        REQUIRE(!logged);
    }
}
