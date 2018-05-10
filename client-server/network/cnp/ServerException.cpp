//
// Created by sckomoroh on 05.05.18.
//

#include "ServerException.h"

using namespace network::cnp;

const char *ServerException::what() const noexcept
{
    return _message.c_str();
}
