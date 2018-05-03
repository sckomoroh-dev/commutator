//
// Created by dev on 4/25/18.
//

#ifndef COMMUTATOR_UDPSERVERSOCKET_H
#define COMMUTATOR_UDPSERVERSOCKET_H

#include <bits/shared_ptr.h>
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
                template<typename TServerIp/**/, typename TPort>
                UdpServerSocket(TServerIp&& serverIp, TPort&& port)
                    : UdpSocket(serverIp, port)
                {
                }

                void bind()
                {
                    if (::bind(_socket,
                               reinterpret_cast<struct sockaddr *>(&_targetSocketAddress),
                               sizeof(_targetSocketAddress)) < 0)
                    {
                        throw SocketException("Unable to bind server socket", errno);
                    }
                }

                std::unique_ptr<UdpClientSocket> readBuffer(void *buffer, size_t bufferSize)
                {
                    struct sockaddr_in clientSocketAddr;

                    socklen_t socketAddressStructureSize = sizeof(clientSocketAddr);
                    if (recvfrom(_socket,
                                 buffer,
                                 bufferSize,
                                 MSG_WAITALL,
                                 reinterpret_cast<struct sockaddr *>(&clientSocketAddr),
                                 &socketAddressStructureSize) < 1)
                    {
                        throw SocketException("Unable to receive data from socket", errno);
                    }

                    return createSocket<UdpClientSocket>(_socket, clientSocketAddr);
                }
            };
        }
    }
}

#endif //COMMUTATOR_UDPSERVERSOCKET_H
