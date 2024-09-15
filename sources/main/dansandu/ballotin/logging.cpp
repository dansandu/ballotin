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
using dansandu::ballotin::string::getFileName;
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
    const auto actualFileName = getFileName(location.file_name());

    const auto lock = std::lock_guard<std::mutex>{mutex_};
    if (level != Level::none && level <= level_)
    {
        const auto logEntry = LogEntry{.timestamp = getLocalDateTime(),
                                       .level = level,
                                       .threadId = std::this_thread::get_id(),
                                       .function = location.function_name(),
                                       .file = actualFileName,
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

void standardOutputLogHandler(const LogEntry& logEntry)
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

struct LogFileHandlerImplementation
{
    explicit LogFileHandlerImplementation(const char* const filePath)
        : logFile{filePath, std::ios_base::out | std::ios_base::app},
          criticalsLogged{false},
          errorsLogged{false},
          warningsLogged{false}
    {
    }

    std::wofstream logFile;
    bool criticalsLogged;
    bool errorsLogged;
    bool warningsLogged;
    mutable std::mutex mutex;
};

LogFileHandler::LogFileHandler(const char* const filePath)
    : implementation_{std::make_shared<LogFileHandlerImplementation>(filePath)}
{
}

void LogFileHandler::operator()(const LogEntry& logEntry) const
{
    const auto message = wformat(logEntry.timestamp, ' ', toString(logEntry.level), ' ', logEntry.threadId, ' ',
                                 logEntry.file, ':', logEntry.line, ' ', logEntry.message, '\n');

    const auto impl = static_cast<LogFileHandlerImplementation*>(implementation_.get());

    const auto lock = std::lock_guard<std::mutex>{impl->mutex};
    switch (logEntry.level)
    {
    case Level::critical:
        impl->criticalsLogged = true;
    case Level::error:
        impl->errorsLogged = true;
    case Level::warn:
        impl->warningsLogged = true;
    default:
        break;
    }

    impl->logFile << message;
}

bool LogFileHandler::criticalsLogged() const
{
    const auto impl = static_cast<LogFileHandlerImplementation*>(implementation_.get());
    const auto lock = std::lock_guard<std::mutex>{impl->mutex};
    return impl->criticalsLogged;
}

bool LogFileHandler::errorsLogged() const
{
    const auto impl = static_cast<LogFileHandlerImplementation*>(implementation_.get());
    const auto lock = std::lock_guard<std::mutex>{impl->mutex};
    return impl->errorsLogged;
}

bool LogFileHandler::warningsLogged() const
{
    const auto impl = static_cast<LogFileHandlerImplementation*>(implementation_.get());
    const auto lock = std::lock_guard<std::mutex>{impl->mutex};
    return impl->warningsLogged;
}

}
