#include "dansandu/ballotin/logging.hpp"
#include "dansandu/ballotin/date_time.hpp"
#include "dansandu/ballotin/exception.hpp"
#include "dansandu/ballotin/file_system.hpp"
#include "dansandu/ballotin/string.hpp"

#include <algorithm>
#include <fstream>

using dansandu::ballotin::date_time::getLocalDateTime;
using dansandu::ballotin::file_system::writeToStandardError;
using dansandu::ballotin::file_system::writeToStandardOutput;
using dansandu::ballotin::string::wformat;

namespace dansandu::ballotin::logging
{

const char* toString(const Level level)
{
    switch (level)
    {
    case Level::none:
        return "NONE";
    case Level::critical:
        return "CRITICAL";
    case Level::error:
        return "ERROR";
    case Level::warn:
        return "WARN";
    case Level::info:
        return "INFO";
    case Level::debug:
        return "DEBUG";
    default:
        THROW(std::logic_error, "Unknown logging level");
    }
}

Logger& Logger::globalInstance()
{
    static auto logger = Logger{};
    return logger;
}

Logger::Logger() : level_{Level::debug}
{
}

void Logger::addHandler(std::wstring name, const Level level, std::function<void(const LogEntry&)> handler)
{
    const auto lock = std::lock_guard<std::mutex>{mutex_};
    if (std::find_if(handlers_.cbegin(), handlers_.cend(),
                     [&name](const auto& handler) { return handler.name == name; }) == handlers_.cend())
    {
        handlers_.push_back({std::move(name), level, std::move(handler)});
    }
    else
    {
        THROW(std::logic_error, "a handler with the same name is already registered");
    }
}

void Logger::removeHandler(const std::wstring_view name)
{
    const auto lock = std::lock_guard<std::mutex>{mutex_};
    if (const auto position = std::find_if(handlers_.cbegin(), handlers_.cend(),
                                           [name](const auto& handler) { return handler.name == name; });
        position != handlers_.cend())
    {
        handlers_.erase(position);
    }
}

void Logger::setLevel(const Level level)
{
    const auto lock = std::lock_guard<std::mutex>{mutex_};
    level_ = level;
}

Level Logger::getLevel() const
{
    const auto lock = std::lock_guard<std::mutex>{mutex_};
    return level_;
}

void Logger::log(const Level level, const std::wstring_view message, const std::source_location location) const
{
    const auto lock = std::lock_guard<std::mutex>{mutex_};
    if (level != Level::none && level <= level_)
    {
        const auto logEntry = LogEntry{.timestamp = getLocalDateTime(),
                                       .level = level,
                                       .threadId = std::this_thread::get_id(),
                                       .function = location.function_name(),
                                       .file = location.file_name(),
                                       .line = static_cast<int>(location.line()),
                                       .column = static_cast<int>(location.column()),
                                       .message = message};

        for (const auto& handler : handlers_)
        {
            if (logEntry.level <= handler.level)
            {
                handler.callback(logEntry);
            }
        }
    }
}

void standardOutputHandler(const LogEntry& logEntry)
{
    const auto message = wformat(logEntry.timestamp, ' ', toString(logEntry.level), ' ', logEntry.threadId, ' ',
                                 logEntry.file, ':', logEntry.line, ' ', logEntry.message, '\n');

    if (logEntry.level < Level::warn)
    {
        writeToStandardError(message);
    }
    else
    {
        writeToStandardOutput(message);
    }
}

struct UnitTestsHandlerImplementation
{
    static void deleter(void* pointer)
    {
        delete static_cast<UnitTestsHandlerImplementation*>(pointer);
    }

    UnitTestsHandlerImplementation(const char* const filePath)
        : logFile{filePath, std::ios_base::out | std::ios_base::app}, errorsLogged{false}, warningsLogged{false}
    {
    }

    std::wofstream logFile;
    bool errorsLogged;
    bool warningsLogged;
    mutable std::mutex mutex;
};

UnitTestsHandler::UnitTestsHandler(const char* const filePath)
    : implementation_{new UnitTestsHandlerImplementation{filePath}, UnitTestsHandlerImplementation::deleter}
{
}

void UnitTestsHandler::operator()(const LogEntry& logEntry)
{
    const auto message = wformat(logEntry.timestamp, ' ', toString(logEntry.level), ' ', logEntry.threadId, ' ',
                                 logEntry.file, ':', logEntry.line, ' ', logEntry.message, '\n');

    const auto casted = static_cast<UnitTestsHandlerImplementation*>(implementation_.get());

    const auto lock = std::lock_guard<std::mutex>{casted->mutex};
    if (logEntry.level == Level::error || logEntry.level == Level::critical)
    {
        casted->errorsLogged = true;
    }
    if (logEntry.level == Level::warn)
    {
        casted->warningsLogged = true;
    }
    casted->logFile << message;
}

bool UnitTestsHandler::errorsLogged() const
{
    const auto casted = static_cast<UnitTestsHandlerImplementation*>(implementation_.get());
    const auto lock = std::lock_guard<std::mutex>{casted->mutex};
    return casted->errorsLogged;
}

bool UnitTestsHandler::warningsLogged() const
{
    const auto casted = static_cast<UnitTestsHandlerImplementation*>(implementation_.get());
    const auto lock = std::lock_guard<std::mutex>{casted->mutex};
    return casted->warningsLogged;
}

}
