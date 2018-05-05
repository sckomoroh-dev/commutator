//
// Created by Anna on 05.05.18.
//

#include "TcpClientSocket.h"

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
        throw SocketException("Unable to connect to server socket", errno);
    }
}

void TcpClientSocket::sendBuffer(void *buffer, size_t bufferSize)
{
    if (send(_socket,
             buffer,
             bufferSize,
             MSG_WAITALL) < 1)
    {
        throw SocketException("Unable to send data to socket", errno);
    }
}

void TcpClientSocket::readBuffer(void *buffer, size_t bufferSize)
{
    if (recv(_socket,
             buffer,
             bufferSize,
             MSG_WAITALL) < 1)
    {
        throw SocketException("Unable to receive data from socket", errno);
    }
}
