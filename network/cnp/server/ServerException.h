//
// Created by sckomoroh on 05.05.18.
//

#ifndef COMMUTATOR_SERVEREXCEPTION_H
#define COMMUTATOR_SERVEREXCEPTION_H

#include <exception>
#include <string>

namespace network
{
    namespace cnp
    {
        namespace server
        {
            class ServerException : public std::exception
            {
            private:
                std::string _message;

            public:
                explicit ServerException(const char *message, int32_t errorCode);

                explicit ServerException(std::string &&message, int32_t errorCode);

                const char *what() const noexcept override;
            };
        }
    }
}

#endif //COMMUTATOR_SERVEREXCEPTION_H
