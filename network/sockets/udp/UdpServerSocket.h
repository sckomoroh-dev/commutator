//
// Created by dev on 4/25/18.
//

#ifndef COMMUTATOR_UDPSERVERSOCKET_H
#define COMMUTATOR_UDPSERVERSOCKET_H

#include <memory>
#include "UdpSocket.h"
#include "UdpClientSocket.h"

namespace network
{
    namespace sockets
    {
        namespace udp
        {
            class UdpServerSocket : public UdpSocket
            {
            public:
                UdpServerSocket(const char* serverIp, int32_t port);

                void bind();

                void sendBuffer(void *buffer, size_t bufferSize, struct sockaddr_in& targetAddress);

                struct sockaddr_in readBuffer(void *buffer, size_t bufferSize);
            };
        }
    }
}

#endif //COMMUTATOR_UDPSERVERSOCKET_H
