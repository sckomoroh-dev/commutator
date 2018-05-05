//
// Created by dev on 4/25/18.
//

#ifndef COMMUTATOR_TCPSOCKET_H
#define COMMUTATOR_TCPSOCKET_H

#include <unistd.h>
#include <cstdint>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cerrno>
#include "../SocketException.h"
#include "../Socket.h"

namespace network
{
    namespace sockets
    {
        namespace tcp
        {
            class TcpSocket : public Socket
            {
            protected:
                TcpSocket(const char* serverIp, int32_t port);

                TcpSocket() = default;
            };
        }
    }
}

#endif //COMMUTATOR_TCPSOCKET_H
