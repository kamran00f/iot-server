#include <cstring>
#include <stdexcept>
#include <string>

#include "dnsUpdater.hpp"

DnsUpdater::DnsUpdater(const int intervalSeconds)
{
    if(intervalSeconds < minUpdateIntervalSeconds)
    {
        std::string excMsg = "Invalid interval, min interval = " + std::to_string(minUpdateIntervalSeconds) +
                             " second(s), given interval = " + std::to_string(intervalSeconds) + " second(s)";
        throw std::runtime_error(excMsg.c_str());
    }

    updateIntervalSeconds = intervalSeconds;
    initCurl();
    updaterThread = std::thread(updateThread, this);
}

DnsUpdater::~DnsUpdater()
{
    if(curl != nullptr)
    {
        curl_easy_cleanup(curl);
        curl = nullptr;
    }

    updaterThread.~thread();
}

std::size_t
    DnsUpdater::copyWebpage(const void *buf, const std::size_t size, const std::size_t nmemb, void *user_pointer)
{
    size_t bytes_num = size * nmemb;
    memcpy(user_pointer, buf, bytes_num);
    return bytes_num;
}

int DnsUpdater::updateDnsIp()
{
    memset(webPageBuffer, 0, sizeof(webPageBuffer));
    auto status = curl_easy_perform(curl);
    if(status == CURLE_OK)
    {
        if(strcmp(webPageBuffer, "OK") == 0)
            return 0;
        else
            log("Unexpected web content received! Received web: " + std::string(webPageBuffer), LogLevel::Debug);
    }
    else
    {
        log("curl_easy_perform() failed with error " + std::to_string(status), LogLevel::Error);
    }

    return -EFAULT;
}

void DnsUpdater::initCurl()
{
    curl = curl_easy_init();
    if(curl == nullptr)
    {
        throw std::runtime_error("Unable to init curl");
    }

    curl_easy_setopt(curl, CURLOPT_URL, dns_url.c_str());     // set URL
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);           // No progress meter
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, webPageBuffer); // set buffer
    curl_easy_setopt(curl,
                     CURLOPT_POSTFIELDSIZE,
                     sizeof(webPageBuffer) - 1); // set buffer len, keep 1 byte for terminating null
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,
                     copyWebpage); // set callback to call after retrieving web page
}

void DnsUpdater::updateThread(DnsUpdater *obj)
{
    while(true)
    {
        int          ret          = obj->updateDnsIp();
        unsigned int sleepSeconds = 0;

        if(ret >= 0)
        {
            obj->log("DNS successfully updated!", LogLevel::Info);
            sleepSeconds = obj->updateIntervalSeconds; // Sleep for updateIntervalSeconds
        }
        else
        {
            obj->log("Updating DNS failed!", LogLevel::Warning);
            sleepSeconds = obj->minUpdateIntervalSeconds; // Sleep for minUpdateIntervalSeconds and try again
        }
        sleep(sleepSeconds);
    }
}