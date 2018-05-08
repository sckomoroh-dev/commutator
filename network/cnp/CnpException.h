//
// Created by sckomoroh on 5/7/18.
//

#ifndef COMMUTATOR_CNPEXCEPTION_H
#define COMMUTATOR_CNPEXCEPTION_H

#include <string>
#include <exception>

namespace network
{
    namespace cnp
    {
        class CnpException : public std::exception
        {
        private:
            std::string _message;

        public:
            explicit CnpException(const char *message);

            explicit CnpException(std::string &&message);

            explicit CnpException(const char *message, int32_t errorCode);

            explicit CnpException(std::string &&message, int32_t errorCode);

            const char *what() const noexcept override;
        };
    }
}

#endif //COMMUTATOR_CNPEXCEPTION_H
