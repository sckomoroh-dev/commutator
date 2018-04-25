//
// Created by dev on 4/24/18.
//

#ifndef COMMUTATOR_TCPSERVERSOCKET_H
#define COMMUTATOR_TCPSERVERSOCKET_H

#include <memory>
#include <cstring>
#include "../SocketException.h"
#include "TcpClientSocket.h"

namespace network
{
    namespace sockets
    {
        namespace tcp
        {
            class TcpServerSocket : public TcpSocket
            {
            public:
                TcpServerSocket(const char *serverIp, uint16_t port)
                    : TcpSocket(serverIp, port)
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

                void listen()
                {
                    if (::listen(_socket, 1) == -1)
                    {
                        throw SocketException("Unable to start listen", errno);
                    }
                }

                std::shared_ptr<TcpClientSocket> accept()
                {
                    struct sockaddr_in clientSocketAddress = {0};
                    auto clientSocketAddressSize = sizeof(clientSocketAddress);

                    auto clientSocketDescriptor = ::accept(_socket,
                                                           reinterpret_cast<struct sockaddr *>(&clientSocketAddress),
                                                           reinterpret_cast<socklen_t *>(&clientSocketAddressSize));

                    if (clientSocketDescriptor == -1)
                    {
                        throw SocketException("Unable to accept client socket", errno);
                    }

                    return std::make_shared<TcpClientSocket>(clientSocketDescriptor, clientSocketAddress);
                }
            };
        }
    }
}

#endif //COMMUTATOR_TCPSERVERSOCKET_H
