#include <iostream>
#include <thread>
#include <stdexcept>

#include "testAppNode.hpp"
#include "server/server.hpp"
#include "message/message.hpp"
#include "utilities/logger.hpp"

AppNode::AppNode(std::string nodeName, std::string serverAddress, uint16_t serverPort) :
    TestNode(serverAddress, serverPort), nodeName(nodeName)
{
    // log("Test app node created, registering node", LogLevel::Debug);
    // Message msg(0, 0, RegisterDevicePayload(0, nodeName));
    // int     ret = sendMessage(msg);
    // if(ret < 0)
    // {
    //     throw std::runtime_error("Error sending RegisterDeviceMessage in AppNode constructor");
    // }

    thread = std::thread(AppNode::threadProcess, this);
}

AppNode::~AppNode()
{
    inDestruction = true;
    shutdown(sock_fd, SHUT_RD);

    if(thread.joinable())
    {
        thread.join();
    }
}

void AppNode::HandleRecievedMessage(const Message &message) const
{
    (void)message;
}

int AppNode::sendMessage(const Message &message) const
{
    size_t sent = send(sock_fd, message.getMessagePointer(), message.getMessageLen(), 0);
    if(sent == message.getMessageLen())
    {
        return 0;
    }
    else
    {
        return -EIO;
    }
}

void AppNode::threadProcess(const AppNode *obj)
{
    (void)obj;
    uint8_t bytes[1024]           = {0};
    bool    connectedNodesQueried = false;

    while(!obj->inDestruction)
    {
        if(!connectedNodesQueried)
        {
            connectedNodesQueried = true;
            obj->log("App node requesting connected nodes", LogLevel::Debug);
            // Message msg(obj->nodeId, 0, GetConnectedNodesPayload());
            // obj->sendMessage(msg);
        }

        int readBytes = read(obj->sock_fd, bytes, sizeof(bytes));
        (void)readBytes;
        // try
        // {
        //     Message message(bytes, readBytes);
        //     if(message.getMessageType() == MessageType::QuickResponse)
        //     {
        //         if(message.getQuickResponsePayload().ack == 0)
        //         {
        //             obj->log("NACK received", LogLevel::Debug);
        //         }
        //         else
        //         {
        //             obj->log("ACK received", LogLevel::Debug);
        //         }
        //     }
        //     else if(message.getMessageType() == MessageType::ConnectedNodeInfo)
        //     {
        //         ConnectedNodeInfoPayload pl = message.getConnectedNodeInfoPayload();
        //         obj->log("App node recived this connected node info: ID=" + std::to_string(pl.nodeId) +
        //                      ", name=" + std::string(pl.nodeName) + ", type=" + std::to_string(pl.NodeType),
        //                  LogLevel::Debug);
        //     }
        //     else
        //     {
        //         obj->log("App node: Something else was received", LogLevel::Debug);
        //     }
        // }
        // catch(const std::exception &e)
        // {
        //     obj->log("App node: Read " + std::to_string(readBytes) + " bytes, but unable to set message!",
        //              LogLevel::Debug);
        // }
    }
}
