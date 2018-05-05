//
// Created by Anna on 05.05.18.
//

#include "UdpServerSocket.h"

using namespace network::sockets::udp;

UdpServerSocket::UdpServerSocket(const char *serverIp, int32_t port)
        : UdpSocket(serverIp, port)
{
}

void UdpServerSocket::bind()
{
    if (::bind(_socket,
               reinterpret_cast<struct sockaddr *>(&_targetSocketAddress),
               sizeof(_targetSocketAddress)) < 0)
    {
        throw SocketException("Unable to bind server socket", errno);
    }
}

struct sockaddr_in UdpServerSocket::readBuffer(void *buffer, size_t bufferSize)
{
    struct sockaddr_in clientSocketAddr;

    socklen_t socketAddressStructureSize = sizeof(clientSocketAddr);
    if (recvfrom(_socket,
                 buffer,
                 bufferSize,
                 MSG_WAITALL,
                 reinterpret_cast<struct sockaddr *>(&clientSocketAddr),
                 &socketAddressStructureSize) < 1)
    {
        throw SocketException("Unable to receive data from socket", errno);
    }

    return clientSocketAddr;
}

void UdpServerSocket::sendBuffer(void *buffer, size_t bufferSize, struct sockaddr_in &targetAddress)
{
    if (sendto(_socket,
               buffer,
               bufferSize,
               MSG_WAITALL,
               reinterpret_cast<struct sockaddr *>(&targetAddress),
               sizeof(targetAddress)) < 1)
    {
        throw SocketException("Unable to send data to socket", errno);
    }
}
