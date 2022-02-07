#pragma once

#include <cstdint>
#include <string>
#include <thread>
#include <functional>

#include "nodeInterface.hpp"
#include "message/message.hpp"
#include "utilities/logger.hpp"

class Node;
typedef std::function<void(const Node *, const Message &)> MessageCallback;
typedef std::function<void(const Node *)>                  DisconnectedCallback;

class Node
{
public:
    Node(const int &fd, const char ip[], MessageCallback messageCallback, DisconnectedCallback disconnectedCallback);
    ~Node();

    bool isRegistered() const { return registered; }
    void setRegistered(bool registered) { registered = registered; }
    // void setInterface(const DeviceInterface::DeviceInterface &nodeInterface) { interface = nodeInterface; }

    void        start();
    uint32_t    getId() const { return id; }
    std::string getName() const { return name; }
    std::string getType() const { return type; }
    std::string getDescription() const { return description; }
    // DeviceInterface::DeviceInterface getInterface() const { return interface; }

    std::string toString() const;
    void        sendMessage(const Message &message) const;

private:
    Node()         = delete;
    using LogLevel = Utilities::Logger::LogLevel;

    int         fd         = -1;
    std::string ip         = "";
    bool        registered = false;

    // These fields are available after registering
    uint32_t    id            = 0;
    uint8_t     macAddress[6] = {0};
    std::string name          = "";
    std::string type          = "";
    std::string description   = "";
    // DeviceInterface::DeviceInterface interface;

    bool                 inDestruction = false;
    std::thread          dataThread;
    MessageCallback      messageCallback;
    DisconnectedCallback disconnectedCallback;

    static void dataThreadProcessor(Node *self);
    void        log(const std::string &message, LogLevel logLevel = LogLevel::Info) const
    {
        Utilities::Logger::logMessage("Node:: " + message, logLevel);
    }
};