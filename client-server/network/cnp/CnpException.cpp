//
// Created by sckomoroh on 5/7/18.
//

#include "CnpException.h"

using namespace network::cnp;

const char *CnpException::what() const noexcept
{
    return exception::what();
}
