#include "testNode.hpp"

TestNode::TestNode(std::string serverAddress, uint16_t serverPort)
{
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port   = htons(serverPort);

    // Convert IPv4 and IPv6 addresses from text to binary form
    inet_pton(AF_INET, serverAddress.c_str(), &serv_addr.sin_addr);

    int connected = connect(sock_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if(connected < 0)
    {
        std::string exceptionMessage = "Unable to connect to " + serverAddress + ":" + std::to_string(serverPort);
        throw std::runtime_error(exceptionMessage);
    }
}

TestNode::~TestNode()
{
    if(sock_fd >= 0)
    {
        close(sock_fd);
    }
}
