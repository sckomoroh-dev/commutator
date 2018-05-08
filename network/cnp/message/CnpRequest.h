//
// Created by sckomoroh on 5/7/18.
//

#ifndef COMMUTATOR_CNPREQUEST_H
#define COMMUTATOR_CNPREQUEST_H

#include <memory>

#include "CnpMessage.h"
#include "../CnpException.h"

namespace network
{
    namespace cnp
    {
        namespace message
        {
            class CnpRequest : public CnpMessage
            {
            private:
                CnpVersion _version;
                std::string _command;
                std::string _data;

            public:
                CnpVersion version() const noexcept;

                const std::string command() const noexcept;

                const std::string data() const noexcept;

                const std::string toString() const final;

                template<typename TRequestString>
                static std::shared_ptr<CnpRequest> fromString(TRequestString &&requestString)
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

                    auto request = std::make_shared<CnpRequest>();
                    request->_version = stringToVersion(mapping["Version"]);
                    request->_command = mapping["Command"];

                    if (mapping.find("Data") != mapping.end())
                    {
                        request->_data = mapping["Data"];
                    }

                    return request;
                }

                template<typename TCommand>
                static std::shared_ptr<CnpRequest> create(CnpVersion version, TCommand &&command)
                {
                    auto request = std::make_shared<CnpRequest>();

                    request->_version = version;
                    request->_command = std::forward<std::string>(command);

                    return request;
                };

                template<typename TCommand, typename TData>
                static std::shared_ptr<CnpRequest> create(CnpVersion version, TCommand&& command, TData&& data)
                {
                    auto request = create(version, command);

                    request->_data = std::forward<std::string>(data);

                    return request;
                };
            };
        }
    }
}

#endif //COMMUTATOR_CNPREQUEST_H
