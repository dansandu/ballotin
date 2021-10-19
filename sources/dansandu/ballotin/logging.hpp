#pragma once

#include "dansandu/ballotin/string.hpp"

#include <atomic>
#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <string_view>
#include <thread>
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

constexpr bool operator<(const Level left, const Level right)
{
    return static_cast<int>(left) < static_cast<int>(right);
}

constexpr bool operator>(const Level left, const Level right)
{
    return right < left;
}

constexpr bool operator<=(const Level left, const Level right)
{
    return !(right < left);
}

constexpr bool operator>=(const Level left, const Level right)
{
    return !(left < right);
}

constexpr const char* levelToString(const Level level)
{
    const char* const levels[] = {"NONE", "ERROR", "WARN", "INFO", "DEBUG"};
    return levels[static_cast<int>(level)];
}

struct LogEntry
{
    std::string_view timestamp;
    Level level;
    std::thread::id threadId;
    const char* function;
    const char* file;
    int line;
    std::function<std::string_view()> messageSupplier;
};

class PRALINE_EXPORT Logger
{
public:
    static Logger& globalInstance();

    Logger();

    void addHandler(std::string name, const Level level, std::function<void(const LogEntry&)> handler);

    void removeHandler(const std::string_view name);

    void setLevel(const Level level);

    Level getLevel() const;

    void log(const Level level, const char* const function, const char* const file, const int line,
             const std::string_view message) const;

    void log(const Level level, const char* const function, const char* const file, const int line,
             const std::function<std::string()>& messageSupplier) const;

private:
    struct Handler
    {
        std::string name;
        Level level;
        std::function<void(const LogEntry&)> callback;
    };

    std::atomic<Level> level_;
    std::vector<Handler> handlers_;
    mutable std::mutex mutex_;
};

PRALINE_EXPORT void standardOutputHandler(const LogEntry& logEntry);

class PRALINE_EXPORT UnitTestsHandler
{
public:
    UnitTestsHandler(const char* const filePath);

    void operator()(const LogEntry& logEntry);

private:
    std::shared_ptr<void> implementation_;
};

}

#if (PRALINE_LOGGING_LEVEL >= 1)
#define LOG_ERROR(...)                                                                                                 \
    dansandu::ballotin::logging::Logger::globalInstance().log(                                                         \
        dansandu::ballotin::logging::Level::error, __func__, __FILE__, __LINE__,                                       \
        [&]() { return dansandu::ballotin::string::format(__VA_ARGS__); });
#else
#define LOG_ERROR(...) ;
#endif

#if (PRALINE_LOGGING_LEVEL >= 2)
#define LOG_WARN(...)                                                                                                  \
    dansandu::ballotin::logging::Logger::globalInstance().log(                                                         \
        dansandu::ballotin::logging::Level::warn, __func__, __FILE__, __LINE__,                                        \
        [&]() { return dansandu::ballotin::string::format(__VA_ARGS__); });
#else
#define LOG_WARN(...) ;
#endif

#if (PRALINE_LOGGING_LEVEL >= 3)
#define LOG_INFO(...)                                                                                                  \
    dansandu::ballotin::logging::Logger::globalInstance().log(                                                         \
        dansandu::ballotin::logging::Level::info, __func__, __FILE__, __LINE__,                                        \
        [&]() { return dansandu::ballotin::string::format(__VA_ARGS__); });
#else
#define LOG_INFO(...) ;
#endif

#if (PRALINE_LOGGING_LEVEL >= 4)
#define LOG_DEBUG(...)                                                                                                 \
    dansandu::ballotin::logging::Logger::globalInstance().log(                                                         \
        dansandu::ballotin::logging::Level::debug, __func__, __FILE__, __LINE__,                                       \
        [&]() { return dansandu::ballotin::string::format(__VA_ARGS__); });
#else
#define LOG_DEBUG(...) ;
#endif
