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
            template<typename TServerIp/**/, typename TPort>
            Socket(TServerIp&& serverIp, TPort&& port)
                : _socket(-1)
                , _targetSocketAddress{0}
            {
                _targetSocketAddress.sin_family = AF_INET;
                _targetSocketAddress.sin_port = htons(port);
                _targetSocketAddress.sin_addr.s_addr = inet_addr(serverIp);
            }

            Socket() = default;

            template<typename TTargetSocketType>
            std::unique_ptr<TTargetSocketType> createSocket(const int32_t& socketDescriptor, const struct sockaddr_in& socketAddress)
            {
                auto targetSocket = std::make_unique<TTargetSocketType>();
                targetSocket->_socket = socketDescriptor;
                targetSocket->_targetSocketAddress = socketAddress;

                return targetSocket;
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
