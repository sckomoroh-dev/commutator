//
// Created by sckomoroh on 06.05.18.
//

#include <sstream>
#include <future>
#include <cstring>
#include <chrono>
#include <thread>
#include "UdpServer.h"

using namespace network::cnp::message;
using namespace network::cnp::server::udp;

UdpServer::UdpServer(const char *serverIp, int32_t port)
    : _udpServerSocket(serverIp, port)
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

void UdpServer::startServer()
{
    _udpServerSocket.bind();
}

void UdpServer::waitInComingRequests()
{
    while (_needStopServer == false)
    {
        struct sockaddr_in targetAddress = { 0 };
        uint32_t messageHeaderLen = readRequestLength(targetAddress);

        if (messageHeaderLen < 1)
        {
            continue;
        }

        auto asyncRes = std::async(std::launch::async, UdpServer::clientMethod, this, std::forward<struct sockaddr_in>(targetAddress), std::forward<uint32_t>(messageHeaderLen));
    }
}

void UdpServer::stopServer()
{
    _needStopServer = true;

    _udpServerSocket.close();
}

void UdpServer::clientMethod(UdpServer *thisPtr, struct sockaddr_in&& targetAddress, uint32_t&& messageLen)
{
    auto request = thisPtr->readRequest(targetAddress, messageLen);

    auto response = thisPtr->getResponse(request);

    std::this_thread::sleep_for(std::chrono::seconds(1));

    thisPtr->sendResponse(response, std::move(targetAddress));
}

uint32_t UdpServer::readRequestLength(struct sockaddr_in &clientSocket)
{
    uint32_t messageHeaderLen = 0;

    clientSocket = _udpServerSocket.readBuffer(static_cast<void*>(&messageHeaderLen),
                                               sizeof(messageHeaderLen));

    return messageHeaderLen;
}

std::shared_ptr<CnpRequest> UdpServer::readRequest(const struct sockaddr_in &targetAddress, uint32_t messageLen)
{
    std::shared_ptr<char> buffer([messageLen]() -> char*
        {
            auto buffer = std::move(static_cast<char *>(malloc(messageLen + 1)));
            memset(buffer, 0, messageLen + 1);

            return buffer;
        }(),
        ::free);

    _udpServerSocket.readBuffer(static_cast<void *>(buffer.get()), messageLen);

    auto request = CnpRequest::fromString(buffer.get());

    return request;
}

std::shared_ptr<CnpResponse> UdpServer::getResponse(const std::shared_ptr<CnpRequest>& request)
{
    if (request->command().empty())
    {
        printf("ERROR: The operation was not specified\n");
        // TODO: throw en exception
    }

    if (request->data().empty())
    {
        printf("The data was not specified\n");
    }

    auto operationMethod = _methodsMap.find(request->command());
    if (operationMethod == _methodsMap.end())
    {
        printf("ERROR: The method implementation was not found\n");
        // TODO: throw an exception
    }

    auto operationResultFunc = operationMethod->second;
    auto operationResult = operationResultFunc(request->data());

    auto response = CnpResponse::create(CnpVersion::Version10, request->command(), operationResult.first, operationResult.second);

    return response;
}
