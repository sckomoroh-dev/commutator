//
// Created by sckomoroh on 4/25/18.
//

#ifndef COMMUTATOR_UDPSERVERSOCKET_H
#define COMMUTATOR_UDPSERVERSOCKET_H

#include "UdpSocket.h"

namespace network::sockets::udp
{
    class UdpServerSocket : public UdpSocket
    {
    public:
        UdpServerSocket(const char* serverIp, int32_t port);

        void bind() const;

        void sendBuffer(void *buffer, size_t bufferSize, struct sockaddr_in& targetAddress) const;

        struct sockaddr_in readBuffer(void *buffer, size_t bufferSize) const;
    };
}

#endif //COMMUTATOR_UDPSERVERSOCKET_H
