#include "dansandu/ballotin/logging.hpp"
#include "dansandu/ballotin/exception.hpp"
#include "dansandu/ballotin/string.hpp"

#include <algorithm>
#include <iostream>

using dansandu::ballotin::date_time::getDateTime;
using dansandu::ballotin::string::format;

namespace dansandu::ballotin::logging
{

const char* levelToString(const Level level)
{
    static const char* const levels[] = {"NONE", "ERROR", "WARN", "INFO", "DEBUG"};
    return levels[levelToInteger(level)];
}

Logger& Logger::globalInstance()
{
    static auto logger = Logger{};
    return logger;
}

Logger::Logger(const Level level, const bool defaultStandardErrorHandler) : level_{level}
{
    if (defaultStandardErrorHandler)
    {
        addHandler("default", Level::warn, standardErrorHandler);
    }
}

void Logger::addHandler(std::string name, const Level level, HandlerType handler)
{
    const auto lock = std::lock_guard<std::mutex>{mutex_};
    if (std::find_if(handlers_.cbegin(), handlers_.cend(),
                     [&name](const auto& handler) { return handler.name == name; }) == handlers_.cend())
    {
        handlers_.push_back({std::move(name), level, std::move(handler)});
    }
    else
    {
        THROW(std::logic_error, "the handler named '", name, "' is already registered");
    }
}

void Logger::removeHandler(const std::string_view name)
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
    level_.store(level);
}

Level Logger::getLevel() const
{
    return level_.load();
}

void Logger::standardErrorHandler(const LogEntry& logEntry)
{
    std::cerr << logEntry.timestamp << " " << levelToString(logEntry.level) << " " << logEntry.function << " "
              << logEntry.file << ":" << logEntry.line << " " << logEntry.message << std::endl;
}

void Logger::log(const LogEntry& logEntry) const
{
    const auto lock = std::lock_guard<std::mutex>{mutex_};
    for (const auto& handler : handlers_)
    {
        if (levelToInteger(logEntry.level) <= levelToInteger(handler.level))
        {
            handler.callback(logEntry);
        }
    }
}

}
