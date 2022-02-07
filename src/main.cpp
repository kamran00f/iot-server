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
            Utilities::Logger::setGlobalLogLevel(Utilities::Logger::LogLevel::Debug);
            Utilities::Logger::logMessage("=== Starting IoT server app ===", Utilities::Logger::LogLevel::Info);
            Server server;

            AppNode testAppNode("Test App Node");

            // Utilities::Logger::logMessage("Starting DNS updater",
            //   Utilities::Logger::LogLevel::Info);
            // Utilities::DnsUpdater dnsUpdater(3600); // Update DNS every 1 hour

            while(true)
            {
                sleep(1000); // stay in main
            }
        }
        catch(const std::exception &e)
        {
            Utilities::Logger::logMessage("Exception! Error: " + std::string(e.what()),
                                          Utilities::Logger::LogLevel::Error);
            sleep(1);
        }
    }
    return 0;
}
