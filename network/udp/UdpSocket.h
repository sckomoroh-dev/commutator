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
                template<typename TServerIp/**/, typename TPort>
                UdpSocket(TServerIp&& serverIp, TPort&& port)
                    : Socket(serverIp, port)
                {
                    _socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
                    if (_socket == -1)
                    {
                        throw SocketException("Cannot to create the socket", errno);
                    }
                }

                UdpSocket() = default;
            };
        }
    }
}

#endif //COMMUTATOR_UDPSOCKET_H
