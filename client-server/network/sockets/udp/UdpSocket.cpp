//
// Created by sckomoroh on 05.05.18.
//

#include "UdpSocket.h"
#include "../SocketException.h"

using namespace network::sockets::udp;

UdpSocket::UdpSocket(const char *serverIp, int32_t port)
    : Socket(serverIp, port)
{
    _socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (_socket == PlatformInvalidSocket)
    {
        throw SocketException("Cannot to create the socket", platform_socket_error());
    }
}
