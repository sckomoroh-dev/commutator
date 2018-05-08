//
// Created by sckomoroh on 5/7/18.
//

#ifndef COMMUTATOR_CNPRESPONSE_H
#define COMMUTATOR_CNPRESPONSE_H

#include "CnpRequest.h"
#include "CnpStatus.h"

class CnpResponse : public CnpMessage
{
private:
    CnpVersion _version;
    std::string _command;
    std::string _data;
    CnpStatus _status;

public:
    CnpVersion version() const noexcept;

    const std::string command() const noexcept;

    const std::string data() const noexcept;

    const CnpStatus status() const noexcept;

    const std::string toString() const final;

    template<typename TRequestString>
    static std::shared_ptr<CnpResponse> fromString(TRequestString&& requestString)
    {
        auto mapping = requestToMap(std::forward<std::string>(requestString));
        if (mapping.find("Version") == mapping.end())
        {
            throw CnpException("Protocol version was not specified");
        }

        if (mapping.find("Command") == mapping.end())
        {
            throw CnpException("A command was not specified");
        }

        if (mapping.find("Status") == mapping.end())
        {
            throw CnpException("A status was not specified");
        }

        auto request = std::make_shared<CnpResponse>();
        request->_version = stringToVersion(mapping["Version"]);
        request->_command = mapping["Command"];
        request->_status = stringToStatus(mapping["Status"]);

        if (mapping.find("Data") != mapping.end())
        {
            request->_data = mapping["Data"];
        }

        return request;
    }

    template<typename TCommand>
    static std::shared_ptr<CnpResponse> create(CnpVersion version, TCommand&& command, CnpStatus status)
    {
        auto request = std::make_shared<CnpResponse>();

        request->_version = version;
        request->_command = command;

        return request;
    };

    template<typename TCommand, typename TData>
    static std::shared_ptr<CnpResponse> create(CnpVersion version, TCommand& command, TData&& data, CnpStatus status)
    {
        auto request = create(version, command, status);

        request->_data = std::forward<std::string>(data);

        return request;
    };

private:
    const std::string statusToString(CnpStatus status) const;

    template<typename TStatusString>
    static const CnpStatus stringToStatus(TStatusString&& statusString)
    {
        std::string statusStr(std::forward(statusString));
        if (statusStr == "OK")
        {
            return CnpStatus::StatusOk;
        }

        if (statusStr == "ERROR")
        {
            return CnpStatus::StatusError;
        }

        throw CnpException("Unknown status");
    }
};

#endif //COMMUTATOR_CNPRESPONSE_H
