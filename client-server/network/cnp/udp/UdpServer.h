//
// Created by sckomoroh on 5/3/18.
//

#ifndef COMMUTATOR_UDPSERVER_H
#define COMMUTATOR_UDPSERVER_H

#include <memory>
#include <map>
#include <atomic>
#include "../message/CnpStatus.h"
#include "../../sockets/udp/UdpServerSocket.h"
#include "../message/CnpRequest.h"
#include "../message/CnpResponse.h"


namespace network::cnp::udp
{
    class UdpServer
    {
    private:
        sockets::udp::UdpServerSocket _socket;
        std::map<std::string, std::function<std::pair<const std::string, message::CnpStatus>(const std::string &)>> _methodsMap;
        std::atomic<bool> _needStopServer;

    public:
        UdpServer(const char *serverIp, int32_t port);

        virtual ~UdpServer() = default;

        void initializeServer();

        void startServer() const;

        void stopServer();

        void waitInComingRequests();

    private:
        static void clientMethod(UdpServer *thisPtr, std::shared_ptr<message::CnpRequest>&& request);

		size_t readRequestLength(struct sockaddr_in &clientSocket) const;

        std::shared_ptr<message::CnpRequest> readRequest(const struct sockaddr_in &targetAddress, size_t messageLen) const;

        std::shared_ptr<message::CnpResponse> getResponse(const std::shared_ptr<message::CnpRequest> &request);

        template<typename TResponse>
        void sendResponse(TResponse&& response, struct sockaddr_in&& targetAddress)
        {
            auto responseString = response->toString();

            size_t responseLength = responseString.length();
            _socket.sendBuffer(static_cast<void*>(&responseLength), sizeof(size_t), targetAddress);
            _socket.sendBuffer(static_cast<void*>(const_cast<char*>(responseString.c_str())), responseLength, targetAddress);
        }
    };
}

#endif //COMMUTATOR_UDPSERVER_H
