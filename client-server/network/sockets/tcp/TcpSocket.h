//
// Created by sckomoroh on 4/25/18.
//

#ifndef COMMUTATOR_TCPSOCKET_H
#define COMMUTATOR_TCPSOCKET_H

#include <cstdint>
#include "../Socket.h"

namespace network::sockets::tcp
{
	class TcpSocket : public Socket
	{
	protected:
		TcpSocket(const char* serverIp, int32_t port);

		TcpSocket() = default;
	};
}

#endif //COMMUTATOR_TCPSOCKET_H
