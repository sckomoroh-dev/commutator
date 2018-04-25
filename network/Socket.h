//
// Created by dev on 4/25/18.
//

#ifndef COMMUTATOR_SOCKET_H
#define COMMUTATOR_SOCKET_H

#include <cstdint>
#include <unistd.h>

namespace network
{
    namespace sockets
    {
        class Socket
        {
        protected:
            int32_t _socket;
            struct sockaddr_in _targetSocketAddress;

        protected:
            Socket(const char *serverIp, uint16_t port)
                : _socket(-1)
                , _targetSocketAddress{0}
            {
                _targetSocketAddress.sin_family = AF_INET;
                _targetSocketAddress.sin_port = htons(port);
                _targetSocketAddress.sin_addr.s_addr = inet_addr(serverIp);
            }

            Socket(int32_t socketDescriptor, struct sockaddr_in socketAddress)
                : _socket(socketDescriptor)
                , _targetSocketAddress(socketAddress)
            {
            }

        public:
            void close()
            {
                if (_socket != -1)
                {
                    ::close(_socket);
                    _socket = -1;
                }
            }
        };
    }
}

#endif //COMMUTATOR_SOCKET_H
