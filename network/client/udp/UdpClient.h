//
// Created by dev on 5/3/18.
//

#ifndef COMMUTATOR_UDPCLIENT_H
#define COMMUTATOR_UDPCLIENT_H


#include <cstring>
#include "../../sockets/udp/UdpClientSocket.h"

namespace client
{
    namespace udp
    {
        class UdpClient
        {
        private:
            network::sockets::udp::UdpClientSocket _udpClientSocket;

        public:
            template<typename TServerIp, typename TPort>
            UdpClient(TServerIp &&serverIp, TPort &&port)
                    : _udpClientSocket(serverIp, port)
            {
            }

            void sendHelloMessage()
            {
                char *buffer = static_cast<char *>(malloc(1024));
                buildMessage("Hello server", buffer, 1024);

                _udpClientSocket.sendBuffer(static_cast<void *>(buffer), strlen(buffer));
            }

            template<typename TMessage>
            char *buildMessage(TMessage &&message, char *buffer, size_t bufferSize)
            {
                memset(buffer, 0, bufferSize);

                sprintf(buffer, "CNP v1.0\nMessage: %s\n\n", message);

                return buffer;
            }
        };
    }
}

#endif //COMMUTATOR_UDPCLIENT_H
