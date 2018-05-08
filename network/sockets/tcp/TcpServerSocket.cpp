//
// Created by sckomoroh on 05.05.18.
//

#include "TcpServerSocket.h"

using namespace network::sockets::tcp;

TcpServerSocket::TcpServerSocket(const char *serverIp, int32_t port)
        : TcpSocket(serverIp, port)
{
}

void TcpServerSocket::bind()
{
    if (::bind(_socket,
               reinterpret_cast<struct sockaddr *>(&_targetSocketAddress),
               sizeof(_targetSocketAddress)) < 0)
    {
        throw SocketException("Unable to bind server socket", errno);
    }
}

void TcpServerSocket::listen()
{
    if (::listen(_socket, 1) == -1)
    {
        throw SocketException("Unable to start listen", errno);
    }
}

std::unique_ptr<TcpClientSocket> TcpServerSocket::accept()
{
    struct sockaddr_in clientSocketAddress = {0};
    auto clientSocketAddressSize = sizeof(clientSocketAddress);

    auto clientSocketDescriptor = ::accept(_socket,
                                           reinterpret_cast<struct sockaddr *>(&clientSocketAddress),
                                           reinterpret_cast<socklen_t *>(&clientSocketAddressSize));

    if (clientSocketDescriptor == -1)
    {
        throw SocketException("Unable to accept client socket", errno);
    }

    return createSocket<TcpClientSocket>(clientSocketDescriptor, clientSocketAddress);
}
