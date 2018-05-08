//
// Created by sckomoroh on 4/24/18.
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
                TcpServerSocket(const char* serverIp, int32_t port);

                void bind();

                void listen();

                std::unique_ptr<TcpClientSocket> accept();
            };
        }
    }
}

#endif //COMMUTATOR_TCPSERVERSOCKET_H
