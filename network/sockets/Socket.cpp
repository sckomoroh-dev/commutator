//
// Created by sckomoroh on 05.05.18.
//

#include "Socket.h"
#include "SocketException.h"

using namespace network::sockets;

Socket::Socket(const char* serverIp, int32_t port)
        : _socket(-1)
        , _targetSocketAddress{0}
{
    _targetSocketAddress.sin_family = AF_INET;
    _targetSocketAddress.sin_port = htons(port);
	if (inet_pton(AF_INET, serverIp, &_targetSocketAddress.sin_addr.s_addr) != 1)
	{
		throw SocketException("Invalid network address", errno);
	}
}

struct sockaddr_in Socket::targetAddress() const noexcept
{
	return _targetSocketAddress;
}

void Socket::close()
{
    if (_socket != -1)
    {
		PlatformCloseSocket(_socket);
        _socket = -1;
    }
}
