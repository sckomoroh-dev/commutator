//
// Created by Anna on 05.05.18.
//

#include <sys/socket.h>
#include <arpa/inet.h>
#include <zconf.h>
#include "Socket.h"

using namespace network::sockets;

Socket::Socket(const char* serverIp, int32_t port)
        : _socket(-1)
        , _targetSocketAddress{0}
{
    _targetSocketAddress.sin_family = AF_INET;
    _targetSocketAddress.sin_port = htons(port);
    _targetSocketAddress.sin_addr.s_addr = inet_addr(serverIp);
}

void Socket::close()
{
    if (_socket != -1)
    {
        ::close(_socket);
        _socket = -1;
    }
}
