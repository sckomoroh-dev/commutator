//
// Created by dev on 5/3/18.
//

#ifndef COMMUTATOR_UDPSERVER_H
#define COMMUTATOR_UDPSERVER_H

#include <memory>
#include <atomic>
#include <thread>
#include <future>
#include <list>
#include "../../sockets/udp/UdpServerSocket.h"


namespace server
{
    namespace udp
    {
        class UdpServer
        {
        private:
            network::sockets::udp::UdpServerSocket _udpServerSocket;
            std::atomic<bool> _needStopServer;

        public:
            UdpServer(const char* serverIp, int32_t port)
                    : _udpServerSocket(serverIp, port)
                    , _needStopServer(false)
            {
            }

            virtual ~UdpServer()
            {

            }

            void startServer()
            {
                _udpServerSocket.bind();
            }

            void waitIncommingRequests()
            {
                while (_needStopServer == false)
                {
                    struct sockaddr_in targetAddress;
                    uint32_t messageHeaderLen = readClientMessageLength(targetAddress);

                    if (messageHeaderLen < 1)
                    {
                        continue;
                    }

                    std::async(std::launch::async, clientMethod, this, std::move(targetAddress), messageHeaderLen);
                }
            }

            void stopServer()
            {
                _needStopServer = true;

                _udpServerSocket.close();
            }

        private:
            static void clientMethod(UdpServer *thisPtr, struct sockaddr_in &&targetAddress, uint32_t firstMessageLen)
            {
                auto messageLen = firstMessageLen;

                auto message = thisPtr->readClientMessage(targetAddress, messageLen);

                thisPtr->parseRequest(message);

                // TODO: send response
            }

            uint32_t readClientMessageLength(struct sockaddr_in &clientSocket)
            {
                uint32_t messageHeaderLen;

                clientSocket = _udpServerSocket.readBuffer(static_cast<void *>(&messageHeaderLen),
                                                           sizeof(messageHeaderLen));

                return messageHeaderLen;
            }

            char* readClientMessage(const struct sockaddr_in &targetAddress, uint32_t messageLen)
            {
                char *buffer = static_cast<char *>(malloc(messageLen + 1));
                _udpServerSocket.readBuffer(static_cast<void *>(buffer), messageLen);

                // TODO: do somtethid with buffer like respond
                printf("[SERVER-client] Received:\n----------------------\n\n%s\n--------------------\n\n", buffer);

                return buffer;
            }

            void parseRequest(const char* request)
            {
                std::string requestString(request);
                auto requestFields = splitString(requestString, '\n', false, [](std::string item){ return item; });

                std::list<std::pair<std::string, std::string>> fields;
                for (auto requestField : requestFields)
                {
                    if (requestField.substr(0, 3) == "CNP")
                    {
                        continue;
                    }

                    auto keyValue = parseRequestFields(requestField);

                    fields.push_back(keyValue);
                }
            }

            std::list<std::string> splitString(const std::string& inputString, char delimeter, bool single, std::function<std::string(std::string)> itemProcessingFunc)
            {
                std::list<std::string> result;
                auto pos = 0;
                auto nextPos = 0;

                do
                {
                    pos = inputString.find(delimeter, nextPos);
                    if (pos != std::string::npos)
                    {
                        auto requestItem = inputString.substr(nextPos, pos - nextPos);
                        requestItem = itemProcessingFunc(requestItem);
                        if (requestItem.length() > 0)
                        {
                            result.push_back(requestItem);
                        }

                        nextPos = pos + 1;
                    }
                }
                while (pos != std::string::npos && !single);

                auto requestItem = inputString.substr(nextPos);
                requestItem = itemProcessingFunc(requestItem);
                if (requestItem.length() > 0)
                {
                    result.push_back(requestItem);
                }

                return result;
            }

            std::pair<std::string, std::string> parseRequestFields(std::string requestField)
            {
                auto requestKeyValue = splitString(requestField, ':', true, [](std::string item)
                {
                    auto start = 0;
                    while (item[start] == ' ')
                    {
                        start++;
                    }

                    auto end = item.length();
                    while (item[end-1] == ' ')
                    {
                        end--;
                    }

                    return item.substr(start, end - start);
                });

                auto key = *requestKeyValue.begin();
                auto value = *(++requestKeyValue.begin());

                return std::make_pair<std::string, std::string>(std::move(key), std::move(value));
            };
        };
    }
}

#endif //COMMUTATOR_UDPSERVER_H
