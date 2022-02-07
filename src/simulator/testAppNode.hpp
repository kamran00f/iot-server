#pragma once

#include <string>

#include "server/server.hpp"
#include "testNode.hpp"
#include "message/message.hpp"

class AppNode : private TestNode
{
private:
    using LogLevel = Utilities::Logger::LogLevel;

    std::string nodeName;
    uint32_t    nodeId;
    bool        inDestruction = false;

    int         RegisterNode();
    void        HandleRecievedData(uint8_t *data, size_t len);
    static void socketDataReceiverThread(AppNode *obj);
    void        HandleRecievedMessage(const Message &message) const override;
    int         sendMessage(const Message &message) const override;
    static void threadProcess(const AppNode *object);

    void log(std::string message, LogLevel logLevel = LogLevel::Debug) const
    {
        Utilities::Logger::logMessage("AppNode:: " + message, logLevel);
    }

public:
    AppNode(std::string nodeName, std::string serverAddress = "127.0.0.1", uint16_t serverPort = 10000);
    ~AppNode();
};