//
// Created by Anna on 05.05.18.
//

#include "TcpSocket.h"

using namespace network::sockets::tcp;

TcpSocket::TcpSocket(const char *serverIp, int32_t port)
        : Socket(serverIp, port)
{
    _socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (_socket == -1)
    {
        throw SocketException("Cannot to create the socket", errno);
    }
}
