//
// Created by sckomoroh on 4/24/18.
//

#ifndef COMMUTATOR_TCPCLIENTSOCKET_H
#define COMMUTATOR_TCPCLIENTSOCKET_H

#include "TcpSocket.h"

namespace network::sockets::tcp
{
	class TcpClientSocket : public TcpSocket
	{
	public:
		TcpClientSocket(const char* serverIp, int32_t port);

		TcpClientSocket() = default;

		void connect();

		void sendBuffer(void *buffer, size_t bufferSize) const;

		void readBuffer(void *buffer, size_t bufferSize) const;
	};
}

#endif //COMMUTATOR_TCPCLIENTSOCKET_H
