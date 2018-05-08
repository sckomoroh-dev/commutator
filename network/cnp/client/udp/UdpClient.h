//
// Created by sckomoroh on 5/3/18.
//

#ifndef COMMUTATOR_UDPCLIENT_H
#define COMMUTATOR_UDPCLIENT_H


#include <cstring>
#include <future>
#include "../../message/CnpVersions.h"
#include "../../message/CnpRequest.h"
#include "../../../sockets/udp/UdpClientSocket.h"
#include "../../message/CnpResponse.h"

namespace network
{
    namespace cnp
    {
        namespace client
        {
            namespace udp
            {
                class UdpClient
                {
                private:
                    network::sockets::udp::UdpClientSocket _udpClientSocket;

                public:
                    UdpClient(const char *serverIp, int32_t port);

                    template<typename TCommand>
                    void sendMessage(TCommand&& command)
                    {
                        auto request = message::CnpRequest::create(message::CnpVersion::Version10, std::forward<std::string>(command));

                        sendRequest(std::move(request));
                    }

                    template<typename TCommand, typename TData>
                    void sendMessage(TCommand&& command, TData&& data)
                    {
                        auto request = message::CnpRequest::create(message::CnpVersion::Version10, std::forward<std::string>(command), std::forward<std::string>(data));

                        sendRequest(std::move(request));
                    }

                    template<typename TRequest>
                    void sendRequest(TRequest&& request)
                    {
                        auto requestString = request->toString();
                        uint32_t bufferLen = requestString.length();

                        _udpClientSocket.sendBuffer(static_cast<void*>(&bufferLen), sizeof(uint32_t));

                        _udpClientSocket.sendBuffer(static_cast<void *>(const_cast<char*>(requestString.c_str())), bufferLen);
                    }

                    const std::shared_ptr<message::CnpResponse> readResponse() const;
                };
            }
        }
    }
}

#endif //COMMUTATOR_UDPCLIENT_H
