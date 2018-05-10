//
// Created by sckomoroh on 4/25/18.
//

#ifndef COMMUTATOR_UDPSOCKET_H
#define COMMUTATOR_UDPSOCKET_H

#include <cstdint>

#include "../Socket.h"

namespace network::sockets::udp
{
    class UdpSocket : public Socket
    {
    protected:
        UdpSocket(const char* serverIp, int32_t port);

        UdpSocket() = default;
    };
}

#endif //COMMUTATOR_UDPSOCKET_H
