//
// Created by sckomoroh on 06.05.18.
//

#include <future>
#include <cstring>
#include "UdpServer.h"
#include "../ServerCommandConstants.h"

using namespace server::udp;

UdpServer::UdpServer(const char *serverIp, int32_t port)
    : _udpServerSocket(serverIp, port)
    , _needStopServer(false)
{
}

void UdpServer::initializeServer()
{
    auto getVersionFunc = [](const std::string& input) -> std::pair<std::string, CnpStatus>
    {
        return std::make_pair<std::string, CnpStatus>("v1.0", CnpStatus::StatusOk);
    };

    _methodsMap[QUERY_SERVER_VERSION] = getVersionFunc;
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

        std::async(std::launch::async, clientMethod, this, std::move(targetAddress), messageHeaderLen);
    }
}

void UdpServer::stopServer()
{
    _needStopServer = true;

    _udpServerSocket.close();
}

int UdpServer::clientMethod(UdpServer *thisPtr, struct sockaddr_in &&targetAddress, uint32_t messageLen)
{
    printf("Read client message (Msg len = %d)\n", messageLen);
    auto request = thisPtr->readRequest(targetAddress, messageLen);

    printf("Received from client:\n---------------\n\n%s\n\n---------------\n\n", request->toString().c_str());

    auto response = thisPtr->getResponse(request);
    auto responseString = response->toString();

    uint32_t responseLength = responseString.length();
    thisPtr->_udpServerSocket.sendBuffer(static_cast<void*>(&responseLength), sizeof(uint32_t), targetAddress);
    thisPtr->_udpServerSocket.sendBuffer(static_cast<void*>(const_cast<char*>(responseString.c_str())), responseLength, targetAddress);

    return 0;
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

    printf("Get result for operation: '%s'\n", request->command().c_str());
    auto operationResultFunc = operationMethod->second;
    auto operationResult = operationResultFunc(request->data());

    auto response = CnpResponse::create(CnpVersion::Version10, request->command(), operationResult.first, operationResult.second);
    printf("Server response:\n=======================\n%s\n=======================\n\n", response->toString().c_str());

    return response;
}


