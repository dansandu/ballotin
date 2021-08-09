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
    error,
    warn,
    info,
    debug
};

const char* levelToString(const Level level);

int levelToInteger(const Level level);

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

    Logger(const Level level = Level::warn, const bool defaultStandardErrorHandler = true);

    void addHandler(const std::string name, const Level level, HandlerType handler);

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

}

#define LOG_ERROR(...)                                                                                                 \
    dansandu::ballotin::logging::Logger::globalInstance().log(__func__, __FILE__, __LINE__,                            \
                                                              dansandu::ballotin::logging::Level::error, __VA_ARGS__);

#define LOG_WARN(...)                                                                                                  \
    dansandu::ballotin::logging::Logger::globalInstance().log(__func__, __FILE__, __LINE__,                            \
                                                              dansandu::ballotin::logging::Level::warn, __VA_ARGS__);

#define LOG_INFO(...)                                                                                                  \
    dansandu::ballotin::logging::Logger::globalInstance().log(__func__, __FILE__, __LINE__,                            \
                                                              dansandu::ballotin::logging::Level::info, __VA_ARGS__);

#define LOG_DEBUG(...)                                                                                                 \
    dansandu::ballotin::logging::Logger::globalInstance().log(__func__, __FILE__, __LINE__,                            \
                                                              dansandu::ballotin::logging::Level::debug, __VA_ARGS__);
