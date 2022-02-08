#pragma once

#include <cstdint>
#include <string>
#include <thread>
#include <functional>

#include "node/node.hpp"
//#include "deviceInterface/deviceInterface.hpp"
#include "message/message.hpp"
#include "utilities/logger.hpp"

class ServerNode
{
public:
    ServerNode()  = default;
    ~ServerNode() = default;
    ServerNode(std::string deviceInterfaceString);

    void handleMessage(const Node *node, const Message &message) const;

private:
    using LogLevel = Logger::LogLevel;

    // DeviceInterface::DeviceInterface deviceInterface;

    static void dataThreadProcessor(ServerNode *self);
    void        log(const std::string &message, LogLevel logLevel = LogLevel::Info) const
    {
        Logger::logMessage("ServerNode:: " + message, logLevel);
    }
};