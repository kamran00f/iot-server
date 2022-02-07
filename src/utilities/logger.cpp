#include <iostream>
#include <string>
#include <sstream>
#include <ctime>
#include <iomanip>

#include "logger.hpp"

namespace Utilities
{
bool Logger::logOutputAllowed(Logger::LogLevel requestedLogLevel)
{
    if(requestedLogLevel > globalLogLevel || globalLogLevel == LogLevel::None)
        return false;
    else
        return true;
}

std::string Logger::getLogLevelString(Logger::LogLevel logLevel)
{
    switch(logLevel)
    {
    case LogLevel::Error:
        return "Error";
    case LogLevel::Warning:
        return "Warning";
    case LogLevel::Info:
        return "Info";
    case LogLevel::Debug:
        return "Debug";
    default:
        return "";
    }
    return "";
}

std::string Logger::getLogprefix(Logger::LogLevel logLevel)
{
    using namespace std::chrono;

    auto         currentTime = system_clock::now();
    const time_t t           = system_clock::to_time_t(currentTime);
    struct tm *  now         = localtime(&t);
    auto         millis      = duration_cast<milliseconds>(currentTime.time_since_epoch()).count() % 1000;

    std::stringstream ss;
    ss << std::setfill('0') << std::setw(4) << now->tm_year + 1900 << "-" << std::setw(2) << now->tm_mon + 1 << "-"
       << std::setw(2) << now->tm_mday << " " << std::setw(2) << now->tm_hour << ":" << std::setw(2) << now->tm_min
       << ":" << std::setw(2) << now->tm_sec << "." << std::setw(3) << millis;

    ss << " [" << getLogLevelString(logLevel) << "]: ";

    auto ret_string = ss.str();

    // pad prefix to make the actual test consistent in different log levels
    int padding_spaces = 35 - ret_string.length();
    if(padding_spaces > 0)
    {
        ret_string = ret_string.insert(ret_string.size() - 1, padding_spaces, ' ');
    }
    return ret_string;
}

std::string Logger::getColorPrefix(Logger::LogLevel logLevel)
{
    constexpr static int black   = 30;
    constexpr static int red     = 31;
    constexpr static int green   = 32;
    constexpr static int yellow  = 33;
    constexpr static int blue    = 34;
    constexpr static int magenta = 35;
    constexpr static int cyan    = 36;
    constexpr static int white   = 37;

    (void)black;
    (void)red;
    (void)green;
    (void)yellow;
    (void)blue;
    (void)magenta;
    (void)cyan;
    (void)white;

    constexpr static int effect_end_offset = 20;

    constexpr static int reset       = 0;
    constexpr static int bold_bright = 1;
    constexpr static int underline   = 4;
    constexpr static int inverse     = 7; // Swap foreground and background

    (void)effect_end_offset;
    (void)reset;
    (void)inverse;
    (void)underline;

    /* Example: "\033[1;31mbold red text\033[0m\n"
       \033 is the ESC character, ASCII 27. It is followed by [, then zero or more numbers separated
       by ; and finally the letter m. The numbers describe the colour and format to switch to from
       that point onwards.
    */
    std::string color_prefix = "\033[";

    // Setting variables based on log level
    switch(logLevel)
    {
    case LogLevel::Error:
        color_prefix += std::to_string(bold_bright) + ";" + std::to_string(red) + "m";
        break;
    case LogLevel::Warning:
        color_prefix += std::to_string(bold_bright) + ";" + std::to_string(yellow) + "m";
        break;
    case LogLevel::Info:
        color_prefix += "m";
        break;
    case LogLevel::Debug:
        color_prefix += std::to_string(reset) + ";" + std::to_string(blue) + "m";
        break;
    case LogLevel::None:
        color_prefix += "m";
    }

    return color_prefix;
}

std::string Logger::getColorSuffix(Logger::LogLevel logLevel)
{
    (void)logLevel;
    return "\033[0m";
}

void Logger::logMessage(::std::string message, Logger::LogLevel logLevel)
{
    if(!logOutputAllowed(logLevel))
        return;

    std::stringstream message_ss(message);
    std::string       line;
    std::string       prefix       = getLogprefix(logLevel);
    std::string       color_prefix = getColorPrefix(logLevel);
    std::string       color_suffix = getColorSuffix(logLevel);

    loggerMutex.lock();
    while(std::getline(message_ss, line))
    {
        line = color_prefix + prefix + line + color_suffix;

        if(logLevel == LogLevel::Error)
            ::std::cerr << line << ::std::endl;
        else
            ::std::cout << line << ::std::endl;
    }
    loggerMutex.unlock();
}

} // namespace Utilities
