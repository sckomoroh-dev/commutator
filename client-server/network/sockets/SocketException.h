//
// Created by sckomoroh on 4/24/18.
//

#ifndef COMMUTATOR_SOCKETEXCEPTION_H
#define COMMUTATOR_SOCKETEXCEPTION_H

#include <exception>
#include <sstream>
#include <string>

namespace network::sockets
{
    class SocketException : public std::exception
    {
    private:
        std::string _message;

    public:
		template<typename TMessage>
        explicit SocketException(TMessage&& message, int32_t errorCode)
		{
			char errorBuffer[1024] = { 0 };
			strerror_s<1024>(errorBuffer, errorCode);

			std::stringstream stringStream;
			stringStream << message << " : " << errorBuffer;

			_message = stringStream.str();
		}

        const char *what() const noexcept override;
    };
}

#endif //COMMUTATOR_SOCKETEXCEPTION_H
