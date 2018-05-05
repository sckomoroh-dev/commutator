//
// Created by dev on 4/24/18.
//

#ifndef COMMUTATOR_SOCKETEXCEPTION_H
#define COMMUTATOR_SOCKETEXCEPTION_H

#include <exception>
#include <algorithm>
#include <string>

namespace network
{
    namespace sockets
    {
        class SocketException : public std::exception
        {
        private:
            std::string _message;

        public:
            explicit SocketException(const char *message, int32_t errorCode);

            explicit SocketException(std::string&& message, int32_t errorCode);

            const char *what() const noexcept override;
        };
    }
}

#endif //COMMUTATOR_SOCKETEXCEPTION_H
