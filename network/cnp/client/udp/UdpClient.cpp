//
// Created by Anna on 06.05.18.
//

#include "UdpClient.h"
#include <utility>

using namespace client::udp;

UdpClient::UdpClient(const char *serverIp, int32_t port)
        : _udpClientSocket(serverIp, port)
{
}

void UdpClient::sendMessage(std::string &&command)
{
    auto requestMessage = buildRequestBody(command);

    uint32_t bufferLen = requestMessage.length();

    _udpClientSocket.sendBuffer(static_cast<void*>(&bufferLen), sizeof(uint32_t));

    _udpClientSocket.sendBuffer(static_cast<void *>(const_cast<char*>(requestMessage.c_str())), bufferLen);
}

void UdpClient::sendMessage(std::string &command)
{
    sendMessage(std::move(command));
}

const std::string UdpClient::readResponse()
{
    uint32_t messageLen;

    _udpClientSocket.readBuffer(static_cast<void*>(&messageLen), sizeof(uint32_t));
    if (messageLen < 1)
    {
        // TODO: throw an exception
    }

    char* buffer = static_cast<char*>(malloc(messageLen));
    _udpClientSocket.readBuffer(static_cast<void*>(buffer), messageLen);

    std::string response(buffer);

    free(buffer);

    return response;
}
