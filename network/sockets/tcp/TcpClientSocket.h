//
// Created by sckomoroh on 4/24/18.
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
                TcpClientSocket(const char* serverIp, int32_t port);

                TcpClientSocket() = default;

                void connect();

                void sendBuffer(void *buffer, size_t bufferSize);

                void readBuffer(void *buffer, size_t bufferSize);
            };
        }
    }
}

#endif //COMMUTATOR_TCPCLIENTSOCKET_H
