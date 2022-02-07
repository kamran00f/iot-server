#include <string>
#include <vector>

#include "nodeList.hpp"
#include "node.hpp"


void NodeList::addNode(const Node *node)
{
    nodes.push_back(node);
}


void NodeList::removeNode(const Node *node)
{
    if(node != nullptr)
    {
        std::remove(nodes.begin(), nodes.end(), node);
        delete node;
    }
}

uint32_t NodeList::getAvailableId() const
{
    // First unoccupied id from database
    return minNodeId;
}

Node *NodeList::getNodeById(uint32_t nodeId)
{
    try
    {
        return registeredNodes.at(nodeId);
    }
    catch(const std::exception &e)
    {
        throw std::runtime_error("nodeId not found!");
    }
}

void NodeList::nodeRegistered(Node *node)
{
    (void)node;
}