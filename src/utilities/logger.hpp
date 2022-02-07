#pragma once

#include <mutex>
#include <string>

namespace Utilities
{
class Logger
{
    // TODO: Improve this class, use << or at least variable args for messages
public:
    enum class LogLevel : int
    {
        None = 0,
        Error,
        Warning,
        Info,
        Debug
    };

    static void setGlobalLogLevel(LogLevel newGlobalLogLevel) { globalLogLevel = newGlobalLogLevel; }

    static void logMessage(std::string message, LogLevel logEntryLevel = LogLevel::Info);

private:
    inline static LogLevel   globalLogLevel;
    inline static std::mutex loggerMutex;
    static bool              logOutputAllowed(LogLevel logLevel);
    static std::string       getLogprefix(Logger::LogLevel logLevel);
    static std::string       getLogLevelString(LogLevel LogLevel);
    static std::string       getColorPrefix(LogLevel LogLevel);
    static std::string       getColorSuffix(LogLevel LogLevel);
};
} // namespace Utilities