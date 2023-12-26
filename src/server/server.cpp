#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <semaphore>
#include <functional>
#include <fstream>
#include <sstream>

#include "server.hpp"
#include "serverNodeInterfaceJson.hpp"

Server::Server() : eventSemaphore(0)
{
    serverNode = ServerNode(ServerNodeInterfaceJson);

    addrinfo hints, *p;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family   = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM; // SOCK_STREAM refers to TCP
    hints.ai_flags    = AI_PASSIVE;

    int ret = getaddrinfo(NULL, std::to_string(serverPort).c_str(), &hints, &add_info);
    if(ret != 0)
        throw std::runtime_error("getaddrinfo failed in Server::Server() with error: " + std::to_string(ret));
    if(add_info == nullptr)
        throw std::runtime_error("received null for add_info in Server::Server()");

    // Iterate over addresses and find the IPv4 version
    for(p = add_info; p != nullptr; p = p->ai_next)
    {
        // If address is ipv4 address
        if(p->ai_family == AF_INET)
        {
            sockaddr_in *ipv4 = reinterpret_cast<sockaddr_in *>(p->ai_addr);
            (void)ipv4;
            break;
        }
    }
    if(p == nullptr)
    {
        freeaddrinfo(add_info);
        add_info = nullptr;

        throw std::runtime_error("add_info iteration went to the end without finding any ipv4 in Server::Server()");
    }

    serverSocketFd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
    if(serverSocketFd < 0)
    {
        freeaddrinfo(add_info);
        add_info = nullptr;
        throw std::runtime_error("Socket creation failed in Server::Server() with error: " +
                                 std::to_string(serverSocketFd));
    }

    int option = 1;
    ret        = setsockopt(serverSocketFd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
    if(ret < 0)
    {
        freeaddrinfo(add_info);
        add_info = nullptr;
        throw std::runtime_error("setsockopt for SO_REUSEADDR failed in Server::Server() with error: " +
                                 std::to_string(ret));
    }

    // Binding address to socket
    ret = bind(serverSocketFd, p->ai_addr, p->ai_addrlen);
    if(ret < 0)
    {
        freeaddrinfo(add_info);
        add_info = nullptr;
        close(serverSocketFd);
        throw std::runtime_error("Binding socket failed in Server::Server() with error: " + std::to_string(ret));
    }

    ret = listen(serverSocketFd, 32); // Incoming connections queue len=32
    if(ret < 0)
    {
        freeaddrinfo(add_info);
        add_info = nullptr;
        close(serverSocketFd);
        throw std::runtime_error("Listening to socket failed in Server::Server() with error: " + std::to_string(ret));
    }

    connectionListener = std::thread(Server::connectionListenerProcess, this);
    eventHandler       = std::thread(Server::eventHandlerProcess, this);
}

Server::~Server()
{
    log("Destructing server", LogLevel::Debug);
    inDestruction = true;

    shutdown(serverSocketFd, SHUT_RD);
    if(connectionListener.joinable())
    {
        log("Joining connection listener thread", LogLevel::Debug);
        connectionListener.join();
    }

    eventSemaphore.release();
    if(eventHandler.joinable())
    {
        log("Joining event handler thread", LogLevel::Debug);
        eventHandler.join();
    }

    // Close socket if open
    log("Closing socket", LogLevel::Debug);
    if(serverSocketFd >= 0)
    {
        close(serverSocketFd);
        serverSocketFd = -1;
    }

    // Free add_info in allocated
    log("Free addrinfo", LogLevel::Debug);
    if(add_info != nullptr)
    {
        freeaddrinfo(add_info);
        add_info = nullptr;
    }

    log("Destructor finished", LogLevel::Debug);
}

void Server::connectionListenerProcess(Server *self)
{
    while(!self->inDestruction)
    {
        // Blocking loop listening for incoming connections
        sockaddr_storage client_addr;
        socklen_t        client_addr_size = sizeof(client_addr);
        int              fd               = accept(self->serverSocketFd, (sockaddr *)&client_addr, &client_addr_size);
        if(fd >= 0)
        {
            // Get IP address of accepted connection
            char                ip[INET_ADDRSTRLEN];
            struct sockaddr_in *sockAddressVar = (struct sockaddr_in *)&client_addr;
            inet_ntop(AF_INET, &sockAddressVar->sin_addr, ip, INET_ADDRSTRLEN);
            self->nodeConnectedEvent(fd, ip);
        }
    }
}

void Server::eventHandlerProcess(Server *self)
{
    while(!self->inDestruction)
    {
        self->eventSemaphore.acquire();

        // Handle all queued events
        while(!self->eventQueue.empty())
        {
            self->handleEvent(self->eventQueue.front());
            self->eventQueue.pop();
        }
    }
}

void Server::messageReceivedEvent(const Node *node, const Message &message)
{
    Server::Event newEvent;
    newEvent.type    = Event::EventType::MessageReceived;
    newEvent.node    = const_cast<Node *>(node);
    newEvent.message = new Message(message);
    eventQueue.push(newEvent);
    eventSemaphore.release();
}

void Server::nodeDisconnectedEvent(const Node *node)
{
    Server::Event newEvent;
    newEvent.type = Event::EventType::NodeDisconnected;
    newEvent.node = const_cast<Node *>(node);
    eventQueue.push(newEvent);
    eventSemaphore.release();
}

void Server::nodeConnectedEvent(const int &fd, const char ip[])
{
    Event newEvent;
    newEvent.type = Event::NodeConnected;
    newEvent.node =
        new Node(fd,
                 ip,
                 std::bind(&Server::messageReceivedEvent, this, std::placeholders::_1, std::placeholders::_2),
                 std::bind(&Server::nodeDisconnectedEvent, this, std::placeholders::_1));
    eventQueue.push(newEvent);
    eventSemaphore.release();
}

void Server::handleEvent(Event event)
{
    // TODO: Improve this function
    switch(event.type)
    {
    case Event::Invalid:
        log("Invalid event received!", LogLevel::Error);
        break;

    case Event::NodeConnected:
        nodeList.addNode(event.node);
        event.node->start();
        break;

    case Event::NodeDisconnected:
        nodeList.removeNode(event.node);
        break;

    case Event::MessageReceived:
        if(event.message != nullptr)
        {
            handleMessage(event.node, *event.message);
            delete event.message;
        }
        else
        {
            log("Message event with message == nullptr received in Server::handleEvent", LogLevel::Error);
        }
        break;

    default:
        log("Unknown event received", LogLevel::Error);
    }
}

void Server::handleMessage(const Node *node, const Message &message)
{
    if(node == nullptr)
    {
        log("nullptr in Server::handleMessage", LogLevel::Error);
        return;
    }

    if(message.getDestinationId() == serverId)
    {
        serverNode.handleMessage(node, message);
    }
    else if(node->isRegistered())
    {
        // Message destination is another node, send it to the other node
        try
        {
            Node *destination = nodeList.getNodeById(node->getId());
            destination->sendMessage(message);
        }
        catch(const std::exception &e)
        {
            log(std::string(e.what()) + " in Server::handleMessage", LogLevel::Error);
        }
    }
    else
    {
        log("Unregistered node trying to send message to another node, sender node info: " + node->toString(),
            LogLevel::Warning);
    }
}