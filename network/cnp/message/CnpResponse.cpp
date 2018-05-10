//
// Created by sckomoroh on 5/7/18.
//

#include <sstream>

#include "CnpResponse.h"

using namespace network::cnp::message;

CnpVersion CnpResponse::version() const noexcept
{
    return _version;
}

std::string CnpResponse::command() const noexcept
{
    return _command;
}

std::string CnpResponse::data() const noexcept
{
    return _data;
}

CnpStatus CnpResponse::status() const noexcept
{
    return _status;
}

std::string CnpResponse::toString() const
{
    std::ostringstream stringStream;
    stringStream << "CNP v" << versionToString(_version) << std::endl;
    stringStream << "Command: " << _command << std::endl;
    stringStream << "Status: " << statusToString(_status) << std::endl;

    if (!_data.empty())
    {
        stringStream << "Data: " << _data << std::endl;
    }

    stringStream << std::endl;

    return stringStream.str();
}

std::string CnpResponse::statusToString(CnpStatus status) const
{
    switch(status)
    {
        case CnpStatus::StatusError :
            return "ERROR";

        case CnpStatus::StatusOk :
            return "OK";
    }

    throw CnpException("Invalid status");
}
