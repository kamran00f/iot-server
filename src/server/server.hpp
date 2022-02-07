#pragma once

#include <cstdint>
#include <string>
#include <thread>
#include <netdb.h>
#include <semaphore>
#include <queue>

#include "node/nodeList.hpp"
#include "node/node.hpp"
#include "message/message.hpp"
#include "serverNode.hpp"
#include "utilities/logger.hpp"

class Server
{
public:
    Server();
    ~Server();

private:
    // Constant expressions
    static constexpr uint32_t serverPort = 10000;
    static constexpr uint32_t serverId   = 0;
    using LogLevel                       = Utilities::Logger::LogLevel;

    // TODO: Improve events, use variant maybe
    struct Event
    {
        enum EventType
        {
            Invalid,
            NodeConnected,
            NodeDisconnected,
            MessageReceived,
            Task,
            // And all possible events
        };

        EventType type;

        Node *   node;
        Message *message;

        Event()
        {
            type    = Invalid;
            node    = nullptr;
            message = nullptr;
        }
    };


    // Variables
    int                        serverSocketFd = -1;
    addrinfo *                 add_info       = nullptr;
    std::thread                connectionListener;
    std::thread                eventHandler;
    std::counting_semaphore<1> eventSemaphore;
    std::queue<Event>          eventQueue;
    bool                       inDestruction = false;

    // TaskManager taskManager;
    NodeList   nodeList;
    ServerNode serverNode;

    // Static functions
    static void connectionListenerProcess(Server *self);
    static void eventHandlerProcess(Server *self);

    // Member functions
    std::string getServerInterfaceString() const;
    void        nodeConnectedEvent(const int &fd, const char ip[]);
    void        handleEvent(Event event);
    void        handleMessage(const Node *node, const Message &message);

    // Callbacks
    void messageReceivedEvent(const Node *node, const Message &message);
    void nodeDisconnectedEvent(const Node *node);

    void log(const std::string &message, LogLevel logLevel = LogLevel::Info) const
    {
        Utilities::Logger::logMessage("Server:: " + message, logLevel);
    }
};
