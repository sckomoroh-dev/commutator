//
// Created by sckomoroh on 05.05.18.
//

#include <sstream>
#include <cstring>
#include "SocketException.h"

using namespace network::sockets;

const char *SocketException::what() const noexcept
{
    return _message.c_str();
}
