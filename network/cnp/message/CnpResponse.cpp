//
// Created by sckomoroh on 5/7/18.
//

#include <sstream>
#include <iostream>

#include "CnpResponse.h"

CnpVersion CnpResponse::version() const noexcept
{
    return _version;
}

const std::string CnpResponse::command() const noexcept
{
    return _command;
}

const std::string CnpResponse::data() const noexcept
{
    return _data;
}

const CnpStatus CnpResponse::status() const noexcept
{
    return _status;
}

const std::string CnpResponse::toString() const
{
    std::ostringstream stringStream;
    stringStream << "CNP v" << versionToString(_version) << std::endl;
    stringStream << "Command: " << _command << std::endl;
    stringStream << "Status: " << statusToString(_status) << std::endl;

    if (_data.length() > 0)
    {
        stringStream << "Data: " << _data << std::endl;
    }

    stringStream << std::endl;

    return stringStream.str();
}

const std::string CnpResponse::statusToString(CnpStatus status) const
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
