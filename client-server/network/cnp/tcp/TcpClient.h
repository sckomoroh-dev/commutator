#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include "../../sockets/tcp/TcpClientSocket.h"
#include "../message/CnpResponse.h"

namespace network::cnp::tcp
{
	using ResponsePtr = std::shared_ptr<message::CnpResponse>;

	class TcpClient
	{
	private:
		sockets::tcp::TcpClientSocket _socket;

	public:
		TcpClient(const char* serverIp, int32_t port);
				
		void connect();

		template<typename TCommand>
		void sendMessage(TCommand&& command)
		{
			auto request = message::CnpRequest::create(
				message::CnpVersion::Version10, 
				std::forward<std::string>(command));

			sendRequest(std::move(request));
		}

		template<typename TCommand, typename TData>
		void sendMessage(TCommand&& command, TData&& data)
		{
			auto request = message::CnpRequest::create(
				message::CnpVersion::Version10, 
				std::forward<std::string>(command), 
				std::forward<std::string>(data));

			sendRequest(std::move(request));
		}

		template<typename TRequest>
		void sendRequest(TRequest&& request)
		{
			const auto requestString = request->toString();
			size_t bufferLen = requestString.length();

			_socket.sendBuffer(static_cast<void*>(&bufferLen), sizeof(size_t));

			_socket.sendBuffer(static_cast<void *>(const_cast<char*>(requestString.c_str())), bufferLen);
		}

		ResponsePtr readResponse() const;
	};
}
#endif // TCP_CLIENT_H