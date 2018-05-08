//
// Created by sckomoroh on 4/25/18.
//

#ifndef COMMUTATOR_SOCKET_H
#define COMMUTATOR_SOCKET_H

#include <cstdint>
#include <memory>

namespace network
{
    namespace sockets
    {
        class Socket
        {
        protected:
            int32_t _socket;
            struct sockaddr_in _targetSocketAddress;

        public:
            void close();

        protected:
            Socket() = default;

            Socket(const char* serverIp, int32_t port);

        protected:
            template<typename TTargetSocketType>
            std::unique_ptr<TTargetSocketType> createSocket(const int32_t& socketDescriptor, const struct sockaddr_in& socketAddress)
            {
                auto targetSocket = std::make_unique<TTargetSocketType>();
                targetSocket->_socket = socketDescriptor;
                targetSocket->_targetSocketAddress = socketAddress;

                return targetSocket;
            }
        };
    }
}

#endif //COMMUTATOR_SOCKET_H
