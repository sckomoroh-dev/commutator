#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <map>
#include <atomic>
#include <functional>
#include "../message/CnpStatus.h"
#include "../../sockets/tcp/TcpServerSocket.h"
#include "../message/CnpRequest.h"
#include "../message/CnpResponse.h"

namespace network::cnp::tcp
{
	using ServerSocket = sockets::tcp::TcpServerSocket;
	using ServerSocketMethodsMap = std::map<std::string, std::function<std::pair<const std::string, message::CnpStatus>(const std::string &)>>;
	using ClientSocketPtr = std::shared_ptr<sockets::tcp::TcpClientSocket>;
	using RequestPtr = std::shared_ptr<message::CnpRequest>;
	using ResponsePtr = std::shared_ptr<message::CnpResponse>;

	class TcpServer
	{
	private:
		ServerSocket _socket;
		ServerSocketMethodsMap _methodsMap;
		std::atomic_bool _needStopServer;

	public:
		TcpServer(const char *serverIp, int32_t port);

		virtual ~TcpServer() = default;

		void initializeServer();

		void startServer();

		void stopServer();

		void waitInComingRequests();

	private:
		static void clientMethod(TcpServer *thisPtr, ClientSocketPtr&& clientSocket);

		static size_t readRequestLength(ClientSocketPtr& clientSocket);

		static RequestPtr readRequest(ClientSocketPtr& clientSocket, size_t messageLen);

		ResponsePtr getResponse(const RequestPtr& request);

		template<typename TResponse>
		void sendResponse(ClientSocketPtr& clientSocket, TResponse&& response)
		{
			const auto responseString = response->toString();

			size_t responseLength = responseString.length();
			clientSocket->sendBuffer(static_cast<void*>(&responseLength), sizeof(size_t));
			clientSocket->sendBuffer(static_cast<void*>(const_cast<char*>(responseString.c_str())), responseLength);
		}
	};
}

#endif // TCP_SERVER_H