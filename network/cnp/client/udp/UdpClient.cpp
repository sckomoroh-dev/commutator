//
// Created by sckomoroh on 06.05.18.
//

#include "UdpClient.h"
#include "../../message/CnpRequest.h"
#include <utility>

using namespace client::udp;

UdpClient::UdpClient(const char *serverIp, int32_t port)
        : _udpClientSocket(serverIp, port)
{
}

void UdpClient::sendMessage(std::string &&command)
{
    auto request = CnpRequest::create(CnpVersion::Version10, command);

    auto requestString = request->toString();
    uint32_t bufferLen = requestString.length();

    _udpClientSocket.sendBuffer(static_cast<void*>(&bufferLen), sizeof(uint32_t));

    _udpClientSocket.sendBuffer(static_cast<void *>(const_cast<char*>(requestString.c_str())), bufferLen);
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

    std::shared_ptr<char> buffer(
        [messageLen]() -> char*
        {
            char* buffer = static_cast<char*>(malloc(messageLen + 1));
            memset(buffer, 0, messageLen + 1);
            return buffer;
        }(),
        ::free);

    _udpClientSocket.readBuffer(static_cast<void*>(buffer.get()), messageLen);

    std::string response(buffer.get());

    return response;
}
