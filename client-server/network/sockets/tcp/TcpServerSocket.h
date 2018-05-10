//
// Created by sckomoroh on 4/24/18.
//

#ifndef COMMUTATOR_TCPSERVERSOCKET_H
#define COMMUTATOR_TCPSERVERSOCKET_H

#include <memory>

#include "TcpClientSocket.h"

namespace network::sockets::tcp
{
    class TcpServerSocket : public TcpSocket
    {
    public:
        TcpServerSocket(const char* serverIp, int32_t port);

        void bind();

        void listen() const;

        std::unique_ptr<TcpClientSocket> accept();
    };
}

#endif //COMMUTATOR_TCPSERVERSOCKET_H
