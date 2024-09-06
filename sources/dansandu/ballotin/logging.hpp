#pragma once

#include "dansandu/ballotin/string.hpp"

#include <functional>
#include <memory>
#include <mutex>
#include <source_location>
#include <string>
#include <string_view>
#include <thread>
#include <vector>

namespace dansandu::ballotin::logging
{

enum class Level
{
    none,
    critical,
    error,
    warn,
    info,
    debug
};

const char* toString(const Level level);

struct LogEntry
{
    std::string timestamp;
    Level level;
    std::thread::id threadId;
    const char* function;
    const char* file;
    int line;
    int column;
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

    void log(const Level level, const std::wstring_view message,
             const std::source_location location = std::source_location::current()) const;

private:
    struct Handler
    {
        std::wstring name;
        Level level;
        std::function<void(const LogEntry&)> callback;
    };

    Level level_;
    std::vector<Handler> handlers_;
    mutable std::mutex mutex_;
};

PRALINE_EXPORT void standardOutputHandler(const LogEntry& logEntry);

class PRALINE_EXPORT UnitTestsHandler
{
public:
    explicit UnitTestsHandler(const char* const filePath);

    void operator()(const LogEntry& logEntry);

    bool errorsLogged() const;

    bool warningsLogged() const;

private:
    std::shared_ptr<void> implementation_;
};

template<typename... Arguments>
struct LogCritical
{
    explicit LogCritical(const Arguments&... arguments,
                         const std::source_location location = std::source_location::current())
    {
        Logger::globalInstance().log(Level::critical, dansandu::ballotin::string::wformat(arguments...), location);
    }
};

template<typename... Arguments>
LogCritical(const Arguments&...) -> LogCritical<Arguments...>;

template<typename... Arguments>
struct LogError
{
    explicit LogError(const Arguments&... arguments,
                      const std::source_location location = std::source_location::current())
    {
        Logger::globalInstance().log(Level::error, dansandu::ballotin::string::wformat(arguments...), location);
    }
};

template<typename... Arguments>
LogError(const Arguments&...) -> LogError<Arguments...>;

template<typename... Arguments>
struct LogWarn
{
    explicit LogWarn(const Arguments&... arguments,
                     const std::source_location location = std::source_location::current())
    {
        Logger::globalInstance().log(Level::warn, dansandu::ballotin::string::wformat(arguments...), location);
    }
};

template<typename... Arguments>
LogWarn(const Arguments&...) -> LogWarn<Arguments...>;

template<typename... Arguments>
struct LogInfo
{
    explicit LogInfo(const Arguments&... arguments,
                     const std::source_location location = std::source_location::current())
    {
        Logger::globalInstance().log(Level::info, dansandu::ballotin::string::wformat(arguments...), location);
    }
};

template<typename... Arguments>
LogInfo(const Arguments&...) -> LogInfo<Arguments...>;

template<typename... Arguments>
struct LogDebug
{
    explicit LogDebug(const Arguments&... arguments,
                      const std::source_location location = std::source_location::current())
    {
        Logger::globalInstance().log(Level::debug, dansandu::ballotin::string::wformat(arguments...), location);
    }
};

template<typename... Arguments>
LogDebug(const Arguments&...) -> LogDebug<Arguments...>;

}
