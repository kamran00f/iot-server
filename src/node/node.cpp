#include <iomanip>
#include <sys/socket.h>

#include "node.hpp"
#include "message/message.hpp"

Node::Node(const int &fd, const char ip[], MessageCallback messageCallback, DisconnectedCallback disconnectedCallback) :
    fd(fd), ip(ip), messageCallback(messageCallback), disconnectedCallback(disconnectedCallback)
{
    log("Node created: " + toString(), LogLevel::Debug);
}

Node::~Node()
{
    log("Destructing node: " + toString(), LogLevel::Debug);
    inDestruction = true;

    shutdown(fd, SHUT_RD);

    if(dataThread.joinable())
    {
        log("Joining data thread", LogLevel::Debug);
        dataThread.join();
    }

    log("Destructor finished", LogLevel::Debug);
}

void Node::start()
{
    log("Node started: " + toString(), LogLevel::Debug);
    dataThread = std::thread(dataThreadProcessor, this);
}

std::string Node::toString() const
{
    std::stringstream ss;
    ss << "fd=" << fd << ", ip=" << ip << ", registered=";

    if(registered)
        ss << "true, id=" << id << ", name=" << name << ", type=" << type << ", description=" << description;
    else
        ss << "false";

    return ss.str();
}

void Node::dataThreadProcessor(Node *self)
{
    uint8_t data[Message::maxMessageLen] = {0};
    while(!self->inDestruction)
    {
        int len = read(self->fd, data, Message::maxMessageLen);
        if(len > 0)
        {
            try
            {
                Message msg(data, len);
                self->messageCallback(self, msg);
            }
            catch(const std::exception &e)
            {
                self->log("Unable to create message from incoming data: " + std::string(e.what()), LogLevel::Warning);

                std::stringstream ss;
                ss << "Number of bytes=" << len;
                if(len <= static_cast<int>(Message::maxMessageLen))
                {
                    ss << ", raw data: ";
                    ss << std::hex << std::uppercase << std::setfill('0');
                    for(int i = 0; i < len; i++)
                    {
                        ss << std::setw(2) << (int)data[i] << " ";
                    }
                }

                self->log(ss.str(), LogLevel::Debug);
            }
        }
        else if(len == 0)
        {
            self->disconnectedCallback(self);
            return;
        }
        else
        {
            // Reading incoming data timed out, do nothing
        }
    }
}


void Node::sendMessage(const Message &message) const
{
    if(fd < 0)
    {
        throw std::runtime_error("Invalid fd in Node::sendMessage");
    }

    size_t len = write(fd, message.getMessagePointer(), message.getMessageLen());
    if(len != message.getMessageLen())
    {
        throw std::runtime_error("len != message.getMessageLen() in Node::sendMessage");
    }
}