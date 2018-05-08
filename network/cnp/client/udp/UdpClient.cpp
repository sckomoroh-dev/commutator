//
// Created by sckomoroh on 06.05.18.
//

#include "UdpClient.h"
#include "../../message/CnpRequest.h"
#include <utility>

using namespace network::cnp::message;
using namespace network::cnp::client::udp;

UdpClient::UdpClient(const char *serverIp, int32_t port)
        : _udpClientSocket(serverIp, port)
{
}

const std::shared_ptr<CnpResponse> UdpClient::readResponse() const
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

    return CnpResponse::fromString(buffer.get());
}
