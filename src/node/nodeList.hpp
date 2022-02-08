#pragma once

#include <vector>
#include <map>

#include "node.hpp"

class NodeList
{
public:
    NodeList() {}
    ~NodeList() {}
    void addNode(const Node *node);
    void removeNode(const Node *node);

    uint32_t getAvailableId() const;
    Node *   getNodeById(uint32_t nodeId);
    void     nodeRegistered(Node *node);

private:
    using LogLevel = Logger::LogLevel;

    static constexpr uint32_t minNodeId = 1;

    std::vector<const Node *>  nodes;
    std::map<uint32_t, Node *> registeredNodes;

    void log(const std::string &message, LogLevel logLevel = LogLevel::Info) const
    {
        Logger::logMessage("NodeList:: " + message, logLevel);
    }
};