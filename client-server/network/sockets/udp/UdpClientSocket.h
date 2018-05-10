//
// Created by sckomoroh on 4/25/18.
//

#ifndef COMMUTATOR_UDPCLIENTSOCKET_H
#define COMMUTATOR_UDPCLIENTSOCKET_H

#include <cstdint>
#include "UdpSocket.h"

namespace network::sockets::udp
{
    class UdpClientSocket : public UdpSocket
    {
    public:
        UdpClientSocket(const char* serverIp, int32_t port);

        UdpClientSocket() = default;

        void sendBuffer(void *buffer, size_t bufferSize);

        void readBuffer(void *buffer, size_t bufferSize);
    };
}

#endif //COMMUTATOR_UDPCLIENTSOCKET_H
