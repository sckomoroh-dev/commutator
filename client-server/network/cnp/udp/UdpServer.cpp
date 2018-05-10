//
// Created by sckomoroh on 06.05.18.
//

#include <future>
#include <cstring>
#include <chrono>
#include <thread>
#include "UdpServer.h"
#include "../ServerException.h"

using namespace network::cnp::message;
using namespace network::cnp::udp;

UdpServer::UdpServer(const char *serverIp, int32_t port)
    : _socket(serverIp, port)
    , _needStopServer(false)
{
}

void UdpServer::initializeServer()
{
    auto getVersionFunc = [](const std::string& input) -> std::pair<std::string, CnpStatus>
    {
        return std::make_pair<const std::string, CnpStatus>("v1.0", CnpStatus::StatusOk);
    };

    _methodsMap["GetVersion"] = getVersionFunc;

    auto echoFunc = [](const std::string& input) -> std::pair<std::string, CnpStatus>
    {
        return std::make_pair<const std::string, CnpStatus >(std::move(input), CnpStatus::StatusOk);
    };

    _methodsMap["Echo"] = echoFunc;
}

void UdpServer::startServer() const
{
    _socket.bind();
}

void UdpServer::waitInComingRequests()
{
    while (!_needStopServer)
    {
        struct sockaddr_in targetAddress = { 0 };
	    const auto messageLen = readRequestLength(targetAddress);

        if (messageLen < 1)
        {
            continue;
        }

		auto request = readRequest(targetAddress, messageLen);

        auto thread = std::thread(clientMethod, this, std::move(request));
		thread.detach();
    }
}

void UdpServer::stopServer()
{
    _needStopServer = true;

    _socket.close();
}

void UdpServer::clientMethod(UdpServer *thisPtr, std::shared_ptr<network::cnp::message::CnpRequest>&& request)
{
    auto response = thisPtr->getResponse(request);

    std::this_thread::sleep_for(std::chrono::seconds(1));

    thisPtr->sendResponse(response, std::move(request->targetAddress()));
}

size_t UdpServer::readRequestLength(struct sockaddr_in &clientSocket) const
{
	size_t messageHeaderLen = 0;

    clientSocket = _socket.readBuffer(static_cast<void*>(&messageHeaderLen),
                                      sizeof(messageHeaderLen));

    return messageHeaderLen;
}

std::shared_ptr<CnpRequest> UdpServer::readRequest(const struct sockaddr_in &targetAddress, size_t messageLen) const
{
    std::shared_ptr<char> buffer([messageLen]() -> char*
        {
            auto buffer = static_cast<char *>(malloc(messageLen + 1));
            memset(buffer, 0, messageLen + 1);

            return buffer;
        }(),
        ::free);

    _socket.readBuffer(static_cast<void *>(buffer.get()), messageLen);

    auto request = CnpRequest::fromString(buffer.get());
	request->_targetAddress = targetAddress;

    return request;
}

std::shared_ptr<CnpResponse> UdpServer::getResponse(const std::shared_ptr<CnpRequest>& request)
{
    if (request->command().empty())
    {
        printf("ERROR: The operation was not specified\n");
        
		throw ServerException("The operation was not specified");
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

    auto response = CnpResponse::create(CnpVersion::Version10, request->command(), operationResult.first, operationResult.second);

    return response;
}
