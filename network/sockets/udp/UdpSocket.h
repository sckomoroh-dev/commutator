//
// Created by dev on 4/25/18.
//

#ifndef COMMUTATOR_UDPSOCKET_H
#define COMMUTATOR_UDPSOCKET_H

#include <cstdint>
#include <unistd.h>
#include <cerrno>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../SocketException.h"
#include "../Socket.h"

namespace network
{
    namespace sockets
    {
        namespace udp
        {
            class UdpSocket : public Socket
            {
            protected:
                UdpSocket(const char*  serverIp, int32_t port);

                UdpSocket() = default;
            };
        }
    }
}

#endif //COMMUTATOR_UDPSOCKET_H
