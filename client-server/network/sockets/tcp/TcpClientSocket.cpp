//
// Created by sckomoroh on 05.05.18.
//

#include "TcpClientSocket.h"
#include "../SocketException.h"

using namespace network::sockets::tcp;

TcpClientSocket::TcpClientSocket(const char *serverIp, int32_t port)
        : TcpSocket(serverIp, port)
{
}

void TcpClientSocket::connect()
{
    if (::connect(_socket,
                  reinterpret_cast<struct sockaddr *>(&_targetSocketAddress),
                  sizeof(_targetSocketAddress)) == -1)
    {
        throw SocketException("Unable to connect to server socket", platform_socket_error());
    }
}

void TcpClientSocket::sendBuffer(void *buffer, size_t bufferSize) const
{
    if (platform_send(_socket,
             buffer,
             bufferSize,
             0) < 1)
    {
        throw SocketException("Unable to send data to socket", platform_socket_error());
    }
}

void TcpClientSocket::readBuffer(void *buffer, size_t bufferSize) const
{
    if (platform_recv(_socket,
             buffer,
             bufferSize,
             0) < 1)
    {
        throw SocketException("Unable to receive data from socket", platform_socket_error());
    }
}
