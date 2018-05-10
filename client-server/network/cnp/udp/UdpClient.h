//
// Created by sckomoroh on 5/3/18.
//

#ifndef COMMUTATOR_UDPCLIENT_H
#define COMMUTATOR_UDPCLIENT_H

#include <future>
#include "../message/CnpResponse.h"
#include "../../sockets/udp/UdpClientSocket.h"

namespace network::cnp::udp
{
    class UdpClient
    {
    private:
        sockets::udp::UdpClientSocket _udpClientSocket;

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
            size_t bufferLen = requestString.length();

            _udpClientSocket.sendBuffer(static_cast<void*>(&bufferLen), sizeof(size_t));

            _udpClientSocket.sendBuffer(static_cast<void *>(const_cast<char*>(requestString.c_str())), bufferLen);
        }

        std::shared_ptr<message::CnpResponse> readResponse();
    };
}

#endif //COMMUTATOR_UDPCLIENT_H
