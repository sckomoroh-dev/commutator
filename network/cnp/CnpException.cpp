//
// Created by sckomoroh on 5/7/18.
//

#include <cstring>

#include "CnpException.h"

using namespace network::cnp;

CnpException::CnpException(const char *message)
    : _message(message)
{
}

CnpException::CnpException(std::string&& message)
    : _message(std::move(message))
{
}

CnpException::CnpException(const char *message, int32_t errorCode)
    : _message(std::move(message))
{
    _message += " : ";
    _message += strerror(errorCode);
}

CnpException::CnpException(std::string &&message, int32_t errorCode)
    : _message(std::move(message))
{
    _message += " : ";
    _message += strerror(errorCode);
}

const char *CnpException::what() const noexcept
{
    return exception::what();
}
