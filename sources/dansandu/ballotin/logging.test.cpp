#include "catchorg/catch/catch.hpp"
#include "dansandu/ballotin/logging.hpp"

#include <stdexcept>
#include <string_view>

using dansandu::ballotin::logging::Level;
using dansandu::ballotin::logging::LogEntry;
using dansandu::ballotin::logging::Logger;

TEST_CASE("logging")
{
    SECTION("level matching")
    {
        auto logger = Logger{Level::debug, false};

        const auto expectedFunction = "someFunction";
        const auto expectedFile = "someFile";
        const auto expectedLine = 17;
        const auto expectedLevel = Level::debug;
        const auto expectedMessage = std::string_view{"some message"};

        auto logged = false;

        logger.addHandler("test", Level::debug, [&](const LogEntry& logEntry) {
            REQUIRE(logEntry.function == expectedFunction);

            REQUIRE(logEntry.file == expectedFile);

            REQUIRE(logEntry.line == expectedLine);

            REQUIRE(logEntry.level == expectedLevel);

            REQUIRE(logEntry.message == expectedMessage);

            logged = true;
        });

        logger.log(expectedFunction, expectedFile, expectedLine, expectedLevel, expectedMessage);

        REQUIRE(logged);
    }

    SECTION("level not matching")
    {
        auto logger = Logger{Level::debug, false};

        auto logged = false;

        logger.addHandler("test", Level::info, [&](const LogEntry&) { logged = true; });

        logger.log("function", "file", 3, Level::debug, "message");

        REQUIRE(!logged);
    }

    SECTION("duplicate handler")
    {
        auto logger = Logger{};

        const auto name = "default";
        const auto level = Level::debug;
        const auto handler = [](const LogEntry&) {};

        REQUIRE_THROWS_AS(logger.addHandler(name, level, handler), std::logic_error);
    }
}
