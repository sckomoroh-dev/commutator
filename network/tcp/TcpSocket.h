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
                TcpSocket(const char *serverIp, uint16_t port)
                    : Socket(serverIp, port)
                {
                    _socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
                    if (_socket == -1)
                    {
                        throw SocketException("Cannot to create the socket", errno);
                    }
                }

                TcpSocket(int32_t socketDescriptor, struct sockaddr_in socketAddress)
                    : Socket(socketDescriptor, socketAddress)
                {
                }
            };
        }
    }
}

#endif //COMMUTATOR_TCPSOCKET_H
