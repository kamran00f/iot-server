#include "server/server.hpp"
#include "utilities/logger.hpp"
#include "utilities/dnsUpdater.hpp"
#include "simulator/testAppNode.hpp"

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    while(true)
    {
        // Main application loop
        try
        {
            Logger::setGlobalLogLevel(Logger::LogLevel::Debug);
            Logger::logMessage("=== Starting IoT server app ===", Logger::LogLevel::Info);
            Server server;

            AppNode testAppNode("Test App Node");

            // Logger::logMessage("Starting DNS updater",
            //   Logger::LogLevel::Info);
            // DnsUpdater dnsUpdater(3600); // Update DNS every 1 hour

            while(true)
            {
                sleep(1000); // stay in main
            }
        }
        catch(const std::exception &e)
        {
            Logger::logMessage("Exception! Error: " + std::string(e.what()), Logger::LogLevel::Error);
            sleep(1);
        }
    }
    return 0;
}
