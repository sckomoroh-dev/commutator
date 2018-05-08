//
// Created by sckomoroh on 05.05.18.
//

#include "UdpClientSocket.h"

using namespace network::sockets::udp;

UdpClientSocket::UdpClientSocket(const char *serverIp, int32_t port)
        : UdpSocket(serverIp, port)
{
}

void UdpClientSocket::sendBuffer(void *buffer, size_t bufferSize)
{
    if (sendto(_socket,
               buffer,
               bufferSize,
               MSG_WAITALL,
               reinterpret_cast<struct sockaddr *>(&_targetSocketAddress),
               sizeof(_targetSocketAddress)) < 1)
    {
        throw SocketException("Unable to send data to socket", errno);
    }
}

void UdpClientSocket::readBuffer(void *buffer, size_t bufferSize)
{
    socklen_t socketAddressStructureSize = sizeof(_targetSocketAddress);
    if (recvfrom(_socket,
                 buffer,
                 bufferSize,
                 MSG_WAITALL,
                 reinterpret_cast<struct sockaddr *>(&_targetSocketAddress),
                 &socketAddressStructureSize) < 1)
    {
        throw SocketException("Unable to receive data from socket", errno);
    }
}
