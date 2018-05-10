//
// Created by sckomoroh on 05.05.18.
//

#include "TcpSocket.h"
#include "../SocketException.h"

using namespace network::sockets::tcp;

TcpSocket::TcpSocket(const char *serverIp, int32_t port)
    : Socket(serverIp, port)
{
    _socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (_socket == PlatformInvalidSocket)
    {
        throw SocketException("Cannot to create the socket", errno);
    }
}
