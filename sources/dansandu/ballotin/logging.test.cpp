#include "catchorg/catch/catch.hpp"
#include "dansandu/ballotin/logging.hpp"

#include <stdexcept>
#include <string_view>

using dansandu::ballotin::logging::Level;
using dansandu::ballotin::logging::log;
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

    SECTION("remove handler")
    {
        auto logger = Logger{Level::debug, false};

        auto logged = false;

        logger.addHandler("test", Level::error, [&](const LogEntry&) { logged = true; });

        logger.removeHandler("test");

        logger.log("function", "file", 3, Level::error, "message");

        REQUIRE(!logged);
    }

    SECTION("log")
    {
        auto logger = Logger{Level::debug, false};

        auto logs = std::vector<Level>{};

        logger.addHandler("test", Level::debug, [&logs](const LogEntry& logEntry) { logs.push_back(logEntry.level); });

        const auto function = "function";
        const auto file = "file";
        const auto line = 3;
        const auto message = "my message";

        const std::vector<Level> error = {Level::error};
        const std::vector<Level> errorWarn = {Level::error, Level::warn};
        const std::vector<Level> errorWarnInfo = {Level::error, Level::warn, Level::info};
        const std::vector<Level> errorWarnInfoDebug = {Level::error, Level::warn, Level::info, Level::debug};

        SECTION("none logging level")
        {
            constexpr auto loggingLevel = 0;

            log<Level::error, loggingLevel>(logger, function, file, line, message);

            REQUIRE(logs.empty());

            log<Level::warn, loggingLevel>(logger, function, file, line, message);

            REQUIRE(logs.empty());

            log<Level::info, loggingLevel>(logger, function, file, line, message);

            REQUIRE(logs.empty());

            log<Level::debug, loggingLevel>(logger, function, file, line, message);

            REQUIRE(logs.empty());
        }

        SECTION("error logging level")
        {
            constexpr auto loggingLevel = 1;

            log<Level::error, loggingLevel>(logger, function, file, line, message);

            REQUIRE(logs == error);

            log<Level::warn, loggingLevel>(logger, function, file, line, message);

            REQUIRE(logs == error);

            log<Level::info, loggingLevel>(logger, function, file, line, message);

            REQUIRE(logs == error);

            log<Level::debug, loggingLevel>(logger, function, file, line, message);

            REQUIRE(logs == error);
        }

        SECTION("warn logging level")
        {
            constexpr auto loggingLevel = 2;

            log<Level::error, loggingLevel>(logger, function, file, line, message);

            REQUIRE(logs == error);

            log<Level::warn, loggingLevel>(logger, function, file, line, message);

            REQUIRE(logs == errorWarn);

            log<Level::info, loggingLevel>(logger, function, file, line, message);

            REQUIRE(logs == errorWarn);

            log<Level::debug, loggingLevel>(logger, function, file, line, message);

            REQUIRE(logs == errorWarn);
        }

        SECTION("info logging level")
        {
            constexpr auto loggingLevel = 3;

            log<Level::error, loggingLevel>(logger, function, file, line, message);

            REQUIRE(logs == error);

            log<Level::warn, loggingLevel>(logger, function, file, line, message);

            REQUIRE(logs == errorWarn);

            log<Level::info, loggingLevel>(logger, function, file, line, message);

            REQUIRE(logs == errorWarnInfo);

            log<Level::debug, loggingLevel>(logger, function, file, line, message);

            REQUIRE(logs == errorWarnInfo);
        }

        SECTION("debug logging level")
        {
            constexpr auto loggingLevel = 4;

            log<Level::error, loggingLevel>(logger, function, file, line, message);

            REQUIRE(logs == error);

            log<Level::warn, loggingLevel>(logger, function, file, line, message);

            REQUIRE(logs == errorWarn);

            log<Level::info, loggingLevel>(logger, function, file, line, message);

            REQUIRE(logs == errorWarnInfo);

            log<Level::debug, loggingLevel>(logger, function, file, line, message);

            REQUIRE(logs == errorWarnInfoDebug);
        }
    }
}
