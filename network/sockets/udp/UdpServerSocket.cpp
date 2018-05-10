//
// Created by sckomoroh on 05.05.18.
//

#include "UdpServerSocket.h"
#include "../SocketException.h"

using namespace network::sockets::udp;

UdpServerSocket::UdpServerSocket(const char *serverIp, int32_t port)
        : UdpSocket(serverIp, port)
{
}

void UdpServerSocket::bind() const
{
    if (::bind(_socket,
               reinterpret_cast<struct sockaddr *>(const_cast<struct sockaddr_in*>(&_targetSocketAddress)),
               sizeof(_targetSocketAddress)) < 0)
    {
        throw SocketException("Unable to bind server socket", errno);
    }
}

struct sockaddr_in UdpServerSocket::readBuffer(void *buffer, size_t bufferSize) const
{
    struct sockaddr_in clientSocketAddr = { 0 };

    socklen_t socketAddressStructureSize = sizeof clientSocketAddr;
    if (platform_recvfrom(_socket,
                 buffer,
                 bufferSize,
                 0,
                 &clientSocketAddr,
                 &socketAddressStructureSize) < 1)
    {
        throw SocketException("Unable to receive data from socket", platform_socket_error());
    }

    return clientSocketAddr;
}

void UdpServerSocket::sendBuffer(void *buffer, size_t bufferSize, struct sockaddr_in &targetAddress) const
{
    if (platform_sendto(_socket,
               buffer,
               bufferSize,
               0,
               targetAddress) < 1)
    {
        throw SocketException("Unable to send data to socket", platform_socket_error());
    }
}
