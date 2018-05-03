//
// Created by dev on 4/25/18.
//

#ifndef COMMUTATOR_UDPCLIENTSOCKET_H
#define COMMUTATOR_UDPCLIENTSOCKET_H

#include <cstdint>
#include "UdpSocket.h"

namespace network
{
    namespace sockets
    {
        namespace udp
        {
            class UdpClientSocket : public UdpSocket
            {
            public:
                template<typename TServerIp/**/, typename TPort>
                UdpClientSocket(TServerIp&& serverIp, TPort&& port)
                    : UdpSocket(serverIp, port)
                {
                }

                UdpClientSocket() = default;

                void sendBuffer(void *buffer, size_t bufferSize)
                {
                    if (sendto(_socket,
                               buffer,
                               bufferSize,
                               MSG_WAITALL,
                               reinterpret_cast<struct sockaddr *>(&_targetSocketAddress),
                               sizeof(_targetSocketAddress)) < 1)
                    {
                        throw SocketException("Unable to send data to socket", errno);
                    }
                }

                void readBuffer(void *buffer, size_t bufferSize)
                {
                    socklen_t socketAddressStructureSize = sizeof(_targetSocketAddress);
                    if (recvfrom(_socket,
                                 buffer,
                                 bufferSize,
                                 MSG_WAITALL,
                                 reinterpret_cast<struct sockaddr *>(&_targetSocketAddress),
                                 &socketAddressStructureSize) < 1)
                    {
                        throw SocketException("Unable to receive data from socket", errno);
                    }
                }
            };
        }
    }
}

#endif //COMMUTATOR_UDPCLIENTSOCKET_H
