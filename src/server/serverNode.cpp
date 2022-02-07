#include <vector>

#include "serverNode.hpp"

ServerNode::ServerNode(std::string deviceInterfaceString)
{
    (void)deviceInterfaceString;
    // InterfaceParser interfaceParser;
    // deviceInterface = interfaceParser.parseDeviceInterface(deviceInterfaceString);
}

void ServerNode::handleMessage(const Node *node, const Message &message) const
{
    (void)node;
    (void)message;
}
