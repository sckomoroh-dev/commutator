//
// Created by Anna on 06.05.18.
//

#include <sstream>
#include "UdpServer.h"
#include "../ServerCommandConstants.h"
#include "../../CnpStatuses.h"

using namespace server::udp;

UdpServer::UdpServer(const char *serverIp, int32_t port)
    : _udpServerSocket(serverIp, port)
    , _needStopServer(false)
{
}

void UdpServer::initializeServer()
{
    auto getVersionFunc = [](std::string input) -> std::pair<std::string, std::string>
    {
        return std::make_pair<std::string, std::string>("v1.0", CNP_STATUS_OK);
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
        struct sockaddr_in targetAddress;
        uint32_t messageHeaderLen = readRequestMessageLength(targetAddress);

        if (messageHeaderLen < 1)
        {
            continue;
        }

        std::async(std::launch::async, clientMethod, this, targetAddress, messageHeaderLen);
    }
}

void UdpServer::stopServer()
{
    _needStopServer = true;

    _udpServerSocket.close();
}

void UdpServer::clientMethod(UdpServer *thisPtr, struct sockaddr_in &&targetAddress, uint32_t messageLen)
{
    printf("Read client message (Msg len = %d)\n", messageLen);
    auto message = thisPtr->readRequestMessage(targetAddress, messageLen);

    printf("Received from client:\n---------------\n\n%s\n\n---------------\n\n", message);

    printf("Parsing request\n");
    auto requestParameters = thisPtr->parseRequest(message);

    auto responseString = thisPtr->buildResponseString(requestParameters);

    uint32_t responseLength = responseString.length();
    thisPtr->_udpServerSocket.sendBuffer(static_cast<void*>(&responseLength), sizeof(uint32_t), targetAddress);
    thisPtr->_udpServerSocket.sendBuffer(static_cast<void*>(const_cast<char*>(responseString.c_str())), responseLength, targetAddress);
}

uint32_t UdpServer::readRequestMessageLength(struct sockaddr_in &clientSocket)
{
    uint32_t messageHeaderLen = 0;

    clientSocket = _udpServerSocket.readBuffer(static_cast<void*>(&messageHeaderLen),
                                               sizeof(messageHeaderLen));

    return messageHeaderLen;
}

char *UdpServer::readRequestMessage(const struct sockaddr_in &targetAddress, uint32_t messageLen)
{
    auto *buffer = static_cast<char *>(malloc(messageLen + 1));

    _udpServerSocket.readBuffer(static_cast<void *>(buffer), messageLen);

    return buffer;
}

std::string UdpServer::buildResponseString(std::map<std::string, std::string> requestParameters)
{
    for (auto field : requestParameters)
    {
        printf("'%s' = '%s'\n\n", field.first.c_str(), field.second.c_str());
    }

    auto operation = requestParameters.find("Command");
    if (operation == requestParameters.end())
    {
        printf("ERROR: The operation was not specified\n");
        // TODO: throw en exception
    }

    std::string requestData;
    auto data = requestParameters.find("Data");
    if (data == requestParameters.end())
    {
        printf("The data was not specified\n");
    }
    else
    {
        requestData = data->second;
    }

    auto operationMethod = _methodsMap.find(operation->second);
    if (operationMethod == _methodsMap.end())
    {
        printf("ERROR: The method implementation was not found\n");
        // TODO: throw an exception
    }

    printf("Get result for operation: '%s'\n", operation->second.c_str());
    auto operationResultFunc = operationMethod->second;
    auto operationResult = operationResultFunc(requestData);

    auto responseString = buildResponseBody(operation->second, operationResult.second, operationResult.first);

    printf("Server response:\n=======================\n%s\n=======================\n\n", responseString.c_str());

    return responseString;
}


