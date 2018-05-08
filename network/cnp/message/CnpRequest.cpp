//
// Created by sckomoroh on 5/7/18.
//

#include <iostream>
#include <sstream>

#include "CnpRequest.h"

using namespace network::cnp::message;

CnpVersion CnpRequest::version() const noexcept
{
    return _version;
}

const std::string CnpRequest::command() const noexcept
{
    return _command;
}

const std::string CnpRequest::data() const noexcept
{
    return _data;
}

const std::string CnpRequest::toString() const
{
    std::ostringstream stringStream;
    stringStream << "CNP v" << versionToString(_version) << std::endl;
    stringStream << "Command: " << _command << std::endl;

    if (_data.length() > 0)
    {
        stringStream << "Data: " << _data << std::endl;
    }

    stringStream << std::endl;

    return stringStream.str();
}
