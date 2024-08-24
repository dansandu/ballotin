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

constexpr auto operator<=>(const Level left, const Level right)
{
    return static_cast<int>(left) <=> static_cast<int>(right);
}

constexpr const char* levelToString(const Level level)
{
    const char* const levels[] = {"NONE", "ERROR", "WARN", "INFO", "DEBUG"};
    return levels[static_cast<int>(level)];
}

struct LogEntry
{
    std::string timestamp;
    Level level;
    std::thread::id threadId;
    const char* function;
    const char* file;
    int line;
    std::wstring_view message;
};

class PRALINE_EXPORT Logger
{
public:
    static Logger& globalInstance();

    Logger();

    void addHandler(std::wstring name, const Level level, std::function<void(const LogEntry&)> handler);

    void removeHandler(const std::wstring_view name);

    void setLevel(const Level level);

    Level getLevel() const;

    void log(const Level level, const char* const function, const char* const file, const int line,
             const std::wstring_view message) const;

private:
    struct Handler
    {
        std::wstring name;
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

    bool errorsLogged() const;

    bool warningsLogged() const;

private:
    std::shared_ptr<void> implementation_;
};

}

#if (PRALINE_LOGGING_LEVEL >= 1)
#define LOG_ERROR(...)                                                                                                 \
    dansandu::ballotin::logging::Logger::globalInstance().log(dansandu::ballotin::logging::Level::error, __func__,     \
                                                              __FILE__, __LINE__,                                      \
                                                              dansandu::ballotin::string::wformat(__VA_ARGS__));
#else
#define LOG_ERROR(...) ;
#endif

#if (PRALINE_LOGGING_LEVEL >= 2)
#define LOG_WARN(...)                                                                                                  \
    dansandu::ballotin::logging::Logger::globalInstance().log(dansandu::ballotin::logging::Level::warn, __func__,      \
                                                              __FILE__, __LINE__,                                      \
                                                              dansandu::ballotin::string::wformat(__VA_ARGS__));
#else
#define LOG_WARN(...) ;
#endif

#if (PRALINE_LOGGING_LEVEL >= 3)
#define LOG_INFO(...)                                                                                                  \
    dansandu::ballotin::logging::Logger::globalInstance().log(dansandu::ballotin::logging::Level::info, __func__,      \
                                                              __FILE__, __LINE__,                                      \
                                                              dansandu::ballotin::string::wformat(__VA_ARGS__));
#else
#define LOG_INFO(...) ;
#endif

#if (PRALINE_LOGGING_LEVEL >= 4)
#define LOG_DEBUG(...)                                                                                                 \
    dansandu::ballotin::logging::Logger::globalInstance().log(dansandu::ballotin::logging::Level::debug, __func__,     \
                                                              __FILE__, __LINE__,                                      \
                                                              dansandu::ballotin::string::wformat(__VA_ARGS__));
#else
#define LOG_DEBUG(...) ;
#endif
