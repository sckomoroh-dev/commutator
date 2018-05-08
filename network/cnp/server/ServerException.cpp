//
// Created by sckomoroh on 05.05.18.
//

#include <cstring>
#include "ServerException.h"

using namespace server;

ServerException::ServerException(const char *message, int32_t errorCode)
    : _message(message)
{
    _message += " : ";
    _message += strerror(errorCode);
}

ServerException::ServerException(std::string&& message, int32_t errorCode)
    : _message(std::move(message))
{
    _message += " : ";
    _message += strerror(errorCode);
}

const char *ServerException::what() const noexcept
{
    return _message.c_str();
}
