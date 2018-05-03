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
                template<typename TServerIp/**/, typename TPort>
                TcpSocket(TServerIp&& serverIp, TPort&& port)
                    : Socket(serverIp, port)
                {
                    _socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
                    if (_socket == -1)
                    {
                        throw SocketException("Cannot to create the socket", errno);
                    }
                }

                TcpSocket() = default;
            };
        }
    }
}

#endif //COMMUTATOR_TCPSOCKET_H
