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

namespace network
{
    namespace sockets
    {
        namespace udp
        {
            class UdpSocket : public Socket
            {
            protected:
                UdpSocket(const char *serverIp, uint16_t port)
                    : Socket(serverIp, port)
                {
                    _socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
                    if (_socket == -1)
                    {
                        throw SocketException("Cannot to create the socket", errno);
                    }
                }

                UdpSocket(int32_t socketDescriptor, struct sockaddr_in socketAddress)
                    : Socket(socketDescriptor, socketAddress)
                {
                }
            };
        }
    }
}

#endif //COMMUTATOR_UDPSOCKET_H
