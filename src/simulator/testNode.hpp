#pragma once

#include <string>
#include <arpa/inet.h>
#include <thread>

#include "message/message.hpp"

class TestNode
{
protected:
    int         sock_fd;
    std::thread thread;
    TestNode(std::string serverAddress, uint16_t serverPort);
    ~TestNode();

public:
    virtual void HandleRecievedMessage(const Message &message) const = 0;
    virtual int  sendMessage(const Message &message) const           = 0;
};
