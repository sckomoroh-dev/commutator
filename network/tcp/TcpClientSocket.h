//
// Created by dev on 4/24/18.
//

#ifndef COMMUTATOR_TCPCLIENTSOCKET_H
#define COMMUTATOR_TCPCLIENTSOCKET_H

#include "TcpSocket.h"

namespace network
{
    namespace sockets
    {
        namespace tcp
        {
            class TcpClientSocket : public TcpSocket
            {
            public:
                template<typename TServerIp/**/, typename TPort>
                TcpClientSocket(TServerIp&& serverIp, TPort&& port)
                    : TcpSocket(serverIp, port)
                {
                }

                TcpClientSocket() = default;

                void connect()
                {
                    if (::connect(_socket,
                                  reinterpret_cast<struct sockaddr *>(&_targetSocketAddress),
                                  sizeof(_targetSocketAddress)) == -1)
                    {
                        throw SocketException("Unable to connect to server socket", errno);
                    }
                }

                void sendBuffer(void *buffer, size_t bufferSize)
                {
                    if (send(_socket,
                             buffer,
                             bufferSize,
                             MSG_WAITALL) < 1)
                    {
                        throw SocketException("Unable to send data to socket", errno);
                    }
                }

                void readBuffer(void *buffer, size_t bufferSize)
                {
                    if (recv(_socket,
                             buffer,
                             bufferSize,
                             MSG_WAITALL) < 1)
                    {
                        throw SocketException("Unable to receive data from socket", errno);
                    }
                }
            };
        }
    }
}

#endif //COMMUTATOR_TCPCLIENTSOCKET_H
