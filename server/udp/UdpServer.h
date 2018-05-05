//
// Created by dev on 5/3/18.
//

#ifndef COMMUTATOR_UDPSERVER_H
#define COMMUTATOR_UDPSERVER_H

#include <memory>
#include <atomic>
#include <thread>
#include <list>
#include "../../network/udp/UdpServerSocket.h"

using UdpClientSocketPtr = std::unique_ptr<network::sockets::udp::UdpClientSocket>;

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
            template<typename TServerIp, typename TPort>
            UdpServer(TServerIp &&serverIp, TPort &&port)
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
                    uint32_t messageHeaderLen;
                    auto clientSocket = _udpServerSocket.readBuffer(static_cast<void *>(&messageHeaderLen),
                                                                    sizeof(messageHeaderLen));
                    if (messageHeaderLen < 1)
                    {
                        continue;
                    }

                    std::thread clientThread(clientMethod, this, std::move(clientSocket), messageHeaderLen);
                }
            }

        private:
            static void clientMethod(UdpServer *thisPtr, UdpClientSocketPtr &&clientSocket, uint32_t firstMessageLen)
            {
                uint32_t messageLen = firstMessageLen;

                while (true)
                {
                    thisPtr->readClientMessage(std::move(clientSocket), messageLen);

                    messageLen = thisPtr->readClientMessageLength(std::move(clientSocket));
                    if (messageLen == 0)
                    {
                        break;
                    }
                }
            }

            uint32_t readClientMessageLength(UdpClientSocketPtr &&clientSocket)
            {
                uint32_t messageHeaderLen;

                clientSocket->readBuffer(static_cast<void *>(&messageHeaderLen), sizeof(messageHeaderLen));

                return messageHeaderLen;
            }

            void readClientMessage(UdpClientSocketPtr &&clientSocket, uint32_t messageLen)
            {
                char *buffer = static_cast<char *>(malloc(messageLen + 1));
                clientSocket->readBuffer(static_cast<void *>(buffer), messageLen);

                // TODO: do somtethid with buffer like respond
                printf("[SERVER-client] Received:\n----------------------\n\n%s\n--------------------\n\n", buffer);
            }
        };
    }
}

#endif //COMMUTATOR_UDPSERVER_H
