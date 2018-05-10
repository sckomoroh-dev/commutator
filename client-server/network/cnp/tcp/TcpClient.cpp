#include "TcpClient.h"

#include <cstring>

using namespace network::cnp::tcp;

TcpClient::TcpClient(const char * serverIp, int32_t port)
	: _socket(serverIp, port)
{
}

void TcpClient::connect()
{
	_socket.connect();
}

std::shared_ptr<network::cnp::message::CnpResponse> TcpClient::readResponse() const
{
	uint32_t messageLen;

	_socket.readBuffer(static_cast<void*>(&messageLen), sizeof(uint32_t));
	if (messageLen < 1)
	{
		// TODO: throw an exception
	}

	std::shared_ptr<char> buffer(
		[messageLen]() -> char*
		{
			char* buffer = static_cast<char*>(malloc(messageLen + 1));
			memset(buffer, 0, messageLen + 1);
			return buffer;
		}(),
		::free);

	_socket.readBuffer(static_cast<void*>(buffer.get()), messageLen);

	return message::CnpResponse::fromString(buffer.get());
}
