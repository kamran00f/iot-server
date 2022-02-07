#pragma once

#include <thread>
#include <curl/curl.h>

#include "utilities/logger.hpp"

namespace Utilities
{
class DnsUpdater
{
private:
    static constexpr size_t         webPageBufferSize        = 32;
    static constexpr int            minUpdateIntervalSeconds = 60;
    inline static const std::string dns_url =
        "https://ipv4.cloudns.net/api/dynamicURL/"
        "?q="
        "MzUxMDk5MToyNDQ1Nzc3NjY6YWY3MjZlZjQ3OTY3YTkxNTUyZWY5NTIyMDMxM2M1NjA1YzVjODc5YmM4NzAwZTg5N2"
        "E1OTJlNmE1ZTk4MmMxYg";

    using LogLevel = Utilities::Logger::LogLevel;

    CURL *       curl;
    std::thread  updaterThread;
    char         webPageBuffer[webPageBufferSize];
    unsigned int updateIntervalSeconds;

    int  updateDnsIp();
    void initCurl();

    static std::size_t
                copyWebpage(const void *buf, const std::size_t size, const std::size_t nmemb, void *user_pointer);
    static void updateThread(DnsUpdater *obj);

    void log(const std::string &message, LogLevel logLevel = LogLevel::Info) const
    {
        Utilities::Logger::logMessage("DnsUpdater:: " + message, logLevel);
    }

public:
    DnsUpdater() = delete;
    DnsUpdater(const int intervalSeconds = minUpdateIntervalSeconds);
    ~DnsUpdater();
};
} // namespace Utilities