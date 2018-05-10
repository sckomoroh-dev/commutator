//
// Created by sckomoroh on 05.05.18.
//

#ifndef COMMUTATOR_SERVEREXCEPTION_H
#define COMMUTATOR_SERVEREXCEPTION_H

#include <exception>
#include <sstream>
#include <string>

namespace network::cnp
{
    class ServerException : public std::exception
    {
    private:
        std::string _message;

    public:
		template<typename TMessage>
        explicit ServerException(TMessage&& message, int32_t errorCode)
		{
			char errorBuffer[1024] = { 0 };
			strerror_s<1024>(errorBuffer, errorCode);
			
			std::stringstream stringStream;
			stringStream << std::forward<std::string>(message) << " : " << errorBuffer;

			_message = stringStream.str();
		}

		template<typename TMessage>
		explicit ServerException(TMessage&& message)
			: _message(std::forward<std::string>(message))
		{
		}

        const char *what() const noexcept override;
    };
}

#endif //COMMUTATOR_SERVEREXCEPTION_H
