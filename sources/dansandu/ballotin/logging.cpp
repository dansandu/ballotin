#include "dansandu/ballotin/logging.hpp"
#include "dansandu/ballotin/date_time.hpp"
#include "dansandu/ballotin/exception.hpp"
#include "dansandu/ballotin/string.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>

using dansandu::ballotin::date_time::getDateTime;

namespace dansandu::ballotin::logging
{

Logger& Logger::globalInstance()
{
    static auto logger = Logger{};
    return logger;
}

Logger::Logger() : level_{Level::debug}
{
}

void Logger::addHandler(std::string name, const Level level, std::function<void(const LogEntry&)> handler)
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

void Logger::log(const Level level, const char* const function, const char* const file, const int line,
                 const std::string_view message) const
{
    if (level <= getLevel())
    {
        const auto logEntry = LogEntry{getDateTime(), level, std::this_thread::get_id(),     function,
                                       file,          line,  [message]() { return message; }};

        const auto lock = std::lock_guard<std::mutex>{mutex_};
        for (const auto& handler : handlers_)
        {
            if (logEntry.level <= handler.level)
            {
                handler.callback(logEntry);
            }
        }
    }
}

void Logger::log(const Level level, const char* const function, const char* const file, const int line,
                 const std::function<std::string()>& messageSupplier) const
{
    if (level <= getLevel())
    {
        const auto logEntry = LogEntry{getDateTime(),
                                       level,
                                       std::this_thread::get_id(),
                                       function,
                                       file,
                                       line,
                                       [&messageSupplier, cached = false, message = std::string{}]() mutable
                                       {
                                           if (cached)
                                           {
                                               return std::string_view{message};
                                           }

                                           message = messageSupplier();
                                           cached = true;

                                           return std::string_view{message};
                                       }};

        const auto lock = std::lock_guard<std::mutex>{mutex_};
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
    static auto mutex = std::mutex{};

    const auto lock = std::lock_guard<std::mutex>{mutex};
    if (logEntry.level <= Level::warn)
    {
        std::cerr << logEntry.timestamp << " " << levelToString(logEntry.level) << " " << logEntry.threadId << " "
                  << logEntry.file << ":" << logEntry.line << " " << logEntry.messageSupplier() << std::endl;
    }
    else
    {
        std::cout << logEntry.timestamp << " " << levelToString(logEntry.level) << " " << logEntry.threadId << " "
                  << logEntry.file << ":" << logEntry.line << " " << logEntry.messageSupplier() << std::endl;
    }
}

struct UnitTestsHandlerImplementation
{
    static void deleter(void* pointer)
    {
        delete static_cast<UnitTestsHandlerImplementation*>(pointer);
    }

    UnitTestsHandlerImplementation(const char* const filePath)
        : logFile{filePath, std::ios_base::out | std::ios_base::app}
    {
    }

    std::ofstream logFile;
    std::mutex mutex;
};

UnitTestsHandler::UnitTestsHandler(const char* const filePath)
    : implementation_{new UnitTestsHandlerImplementation{filePath}, UnitTestsHandlerImplementation::deleter}
{
}

void UnitTestsHandler::operator()(const LogEntry& logEntry)
{
    standardOutputHandler(logEntry);

    const auto casted = static_cast<UnitTestsHandlerImplementation*>(implementation_.get());
    const auto lock = std::lock_guard<std::mutex>{casted->mutex};
    casted->logFile << logEntry.timestamp << " " << levelToString(logEntry.level) << " " << logEntry.threadId << " "
                    << logEntry.file << ":" << logEntry.line << " " << logEntry.messageSupplier() << std::endl;
}

}
