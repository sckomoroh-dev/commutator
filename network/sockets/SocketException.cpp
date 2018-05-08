//
// Created by sckomoroh on 05.05.18.
//

#include <cstring>
#include "SocketException.h"

using namespace network::sockets;

SocketException::SocketException(const char *message, int32_t errorCode)
        : _message(message)
{
    _message += " : ";
    _message += strerror(errorCode);
}

SocketException::SocketException(std::string&& message, int32_t errorCode)
        : _message(std::move(message))
{
    _message += " : ";
    _message += strerror(errorCode);
}

const char *SocketException::what() const noexcept
{
    return _message.c_str();
}
