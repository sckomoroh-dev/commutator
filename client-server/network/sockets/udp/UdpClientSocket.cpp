//
// Created by sckomoroh on 05.05.18.
//

#include "UdpClientSocket.h"
#include "../SocketException.h"

using namespace network::sockets::udp;

UdpClientSocket::UdpClientSocket(const char *serverIp, int32_t port)
        : UdpSocket(serverIp, port)
{
}

void UdpClientSocket::sendBuffer(void *buffer, size_t bufferSize)
{
	const auto sendtoResult = platform_sendto(_socket,
		buffer,
		bufferSize,
		0,
		_targetSocketAddress);

    if (sendtoResult < 1)
    {
        throw SocketException("Unable to send data to socket", platform_socket_error());
    }
}

void UdpClientSocket::readBuffer(void *buffer, size_t bufferSize)
{
    socklen_t socketAddressStructureSize = sizeof _targetSocketAddress ;
    if (platform_recvfrom(_socket,
                 buffer,
                 bufferSize,
                 0,
                 &_targetSocketAddress,
                 &socketAddressStructureSize) < 1)
    {
        throw SocketException("Unable to receive data from socket", platform_socket_error());
    }
}
