#include "TcpServer.h"
#include <thread>
#include "../ServerException.h"

using namespace network::cnp::tcp;

TcpServer::TcpServer(const char *serverIp, int32_t port)
	: _socket(serverIp, port)
	, _needStopServer(false)
{
}

void TcpServer::initializeServer()
{
	auto getVersionFunc = [](const std::string& input) -> std::pair<std::string, message::CnpStatus>
	{
		return std::make_pair<const std::string, message::CnpStatus>("v1.0", message::CnpStatus::StatusOk);
	};

	_methodsMap["GetVersion"] = getVersionFunc;

	auto echoFunc = [](const std::string& input) -> std::pair<std::string, message::CnpStatus>
	{
		return std::make_pair<const std::string, message::CnpStatus>(std::move(input), message::CnpStatus::StatusOk);
	};

	_methodsMap["Echo"] = echoFunc;
}

void TcpServer::startServer()
{
	_socket.bind();
	_socket.listen();
}

void TcpServer::waitInComingRequests()
{
	while (!_needStopServer)
	{
		auto clientSocket = _socket.accept();
		auto thread = std::thread(clientMethod, this, std::move(clientSocket));
		thread.detach();
	}
}

void TcpServer::stopServer()
{
	_needStopServer = true;

	_socket.close();
}

void TcpServer::clientMethod(TcpServer *thisPtr, ClientSocketPtr&& clientSocket)
{
	const auto messageLen = readRequestLength(clientSocket);
	if (messageLen < 1)
	{
		return;
	}

	const auto request = readRequest(clientSocket, messageLen);

	auto response = thisPtr->getResponse(request);

	thisPtr->sendResponse(clientSocket, response);
}

size_t TcpServer::readRequestLength(ClientSocketPtr& clientSocket)
{
	size_t messageHeaderLen = 0;

	clientSocket->readBuffer(static_cast<void*>(&messageHeaderLen),  sizeof(size_t));

	return messageHeaderLen;
}

RequestPtr TcpServer::readRequest(ClientSocketPtr& clientSocket, size_t messageLen)
{
	std::shared_ptr<char> buffer([messageLen]() -> char*
	{
		auto buffer = static_cast<char *>(malloc(messageLen + 1));
		memset(buffer, 0, messageLen + 1);

		return buffer;
	}(),
		::free);

	clientSocket->readBuffer(static_cast<void *>(buffer.get()), messageLen);

	auto request = message::CnpRequest::fromString(buffer.get());

	return request;
}

ResponsePtr TcpServer::getResponse(const RequestPtr& request)
{
	if (request->command().empty())
	{
		printf("ERROR: The operation was not specified\n");
		
		throw ServerException("ERROR: The operation was not specified\n");
	}

	if (request->data().empty())
	{
		printf("The data was not specified\n");
	}

	const auto operationMethod = _methodsMap.find(request->command());
	if (operationMethod == _methodsMap.end())
	{
		printf("ERROR: The method implementation was not found\n");
	
		throw ServerException("The method implementation was not found");
	}

	const auto operationResultFunc = operationMethod->second;
	const auto operationResult = operationResultFunc(request->data());

	return message::CnpResponse::create(message::CnpVersion::Version10, request->command(), operationResult.first, operationResult.second);
}

