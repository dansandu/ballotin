#pragma once

#include "dansandu/ballotin/date_time.hpp"
#include "dansandu/ballotin/string.hpp"

#include <atomic>
#include <functional>
#include <mutex>
#include <string_view>
#include <vector>

namespace dansandu::ballotin::logging
{

enum class Level
{
    none,
    error,
    warn,
    info,
    debug
};

const char* levelToString(const Level level);

constexpr int levelToInteger(const Level level)
{
    return static_cast<int>(level);
}

struct LogEntry
{
    std::string_view timestamp;
    Level level;
    const char* function;
    const char* file;
    int line;
    std::string_view message;
};

class Logger
{
public:
    using HandlerType = std::function<void(const LogEntry&)>;

    static Logger& globalInstance();

    Logger(const Level level = Level::debug, const bool defaultStandardErrorHandler = true);

    void addHandler(std::string name, const Level level, HandlerType handler);

    void removeHandler(const std::string_view name);

    void setLevel(const Level level);

    Level getLevel() const;

    template<typename... Arguments>
    void log(const char* const function, const char* const file, const int line, const Level level,
             Arguments&&... arguments) const
    {
        if (levelToInteger(level) <= levelToInteger(getLevel()))
        {
            const auto logEntry =
                LogEntry{.timestamp = dansandu::ballotin::date_time::getDateTime(),
                         .level = level,
                         .function = function,
                         .file = file,
                         .line = line,
                         .message = dansandu::ballotin::string::format(std::forward<Arguments>(arguments)...)};

            log(logEntry);
        }
    }

private:
    struct Handler
    {
        std::string name;
        Level level;
        HandlerType callback;
    };

    static void standardErrorHandler(const LogEntry& logEntry);

    void log(const LogEntry& logEntry) const;

    std::atomic<Level> level_;
    std::vector<Handler> handlers_;
    mutable std::mutex mutex_;
};

template<Level level, int loggingLevel, typename... Arguments>
void log(const Logger& logger, const char* const function, const char* const file, const int line,
         Arguments&&... arguments)
{
    if constexpr (levelToInteger(level) <= loggingLevel)
    {
        logger.log(function, file, line, level, std::forward<Arguments>(arguments)...);
    }
}

}

#define LOG_ERROR(...)                                                                                                 \
    dansandu::ballotin::logging::log<dansandu::ballotin::logging::Level::error, PRALINE_LOGGING_LEVEL>(                \
        Logger::globalInstance(), __func__, __FILE__, __LINE__, __VA_ARGS__);

#define LOG_WARN(...)                                                                                                  \
    dansandu::ballotin::logging::log<dansandu::ballotin::logging::Level::warn, PRALINE_LOGGING_LEVEL>(                 \
        Logger::globalInstance(), __func__, __FILE__, __LINE__, __VA_ARGS__);

#define LOG_INFO(...)                                                                                                  \
    dansandu::ballotin::logging::log<dansandu::ballotin::logging::Level::info, PRALINE_LOGGING_LEVEL>(                 \
        Logger::globalInstance(), __func__, __FILE__, __LINE__, __VA_ARGS__);

#define LOG_DEBUG(...)                                                                                                 \
    dansandu::ballotin::logging::log<dansandu::ballotin::logging::Level::debug, PRALINE_LOGGING_LEVEL>(                \
        Logger::globalInstance(), __func__, __FILE__, __LINE__, __VA_ARGS__);
