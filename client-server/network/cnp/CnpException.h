//
// Created by sckomoroh on 5/7/18.
//

#ifndef COMMUTATOR_CNPEXCEPTION_H
#define COMMUTATOR_CNPEXCEPTION_H

#include <sstream>
#include <string>
#include <exception>

namespace network::cnp
{
    class CnpException : public std::exception
    {
    private:
        std::string _message;

    public:
		template<typename TMessage>
        explicit CnpException(TMessage&& message)
			: _message(std::forward<std::string>(message))
		{
		}

		template<typename TMessage>
        explicit CnpException(TMessage&& message, int32_t errorCode)
		{
			char errorBuffer[1024] = { 0 };
			strerror_s<1023>(errorBuffer, errorCode);

			std::stringstream stringStream;
			stringStream << std::forward<std::string>(message) << " : " << errorBuffer;

			_message = stringStream.str();
		}

        const char *what() const noexcept override;
    };
}

#endif //COMMUTATOR_CNPEXCEPTION_H
