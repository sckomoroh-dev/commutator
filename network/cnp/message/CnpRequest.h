//
// Created by sckomoroh on 5/7/18.
//

#ifndef COMMUTATOR_CNPREQUEST_H
#define COMMUTATOR_CNPREQUEST_H

#include <memory>

#include "../../sockets/platform/SocketPlatform.h"
#include "CnpMessage.h"
#include "../CnpException.h"

namespace network
{
    namespace cnp
    {
		namespace server
		{
			namespace udp
			{
				class UdpServer;
			}
		}

        namespace message
        {

            class CnpRequest : public CnpMessage
            {
            private:
                CnpVersion _version;
                std::string _command;
                std::string _data;
				struct sockaddr_in _targetAddress;

            public:
				template<typename TCommand>
				CnpRequest(CnpVersion version, TCommand&& command)
					: _version(version)
					, _command(std::forward<std::string>(command))
				{
				}

                CnpVersion version() const noexcept;

                std::string command() const noexcept;

                std::string data() const noexcept;

                std::string toString() const final;

				struct sockaddr_in targetAddress() const noexcept;

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

                    auto request = std::make_shared<CnpRequest>(stringToVersion(mapping["Version"]), mapping["Command"]);
                    if (mapping.find("Data") != mapping.end())
                    {
                        request->_data = mapping["Data"];
                    }

                    return request;
                }

                template<typename TCommand>
                static std::shared_ptr<CnpRequest> create(CnpVersion version, TCommand &&command)
                {
                    auto request = std::make_shared<CnpRequest>(version, command);

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

				friend class server::udp::UdpServer;
            };
        }
    }
}

#endif //COMMUTATOR_CNPREQUEST_H
