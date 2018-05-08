//
// Created by sckomoroh on 5/3/18.
//

#ifndef COMMUTATOR_UDPSERVER_H
#define COMMUTATOR_UDPSERVER_H

#include <memory>
#include <map>
#include <atomic>
#include "../../../sockets/udp/UdpServerSocket.h"
#include "../../message/CnpRequest.h"
#include "../../message/CnpResponse.h"


namespace server
{
    namespace udp
    {
        class UdpServer
        {
        private:
            network::sockets::udp::UdpServerSocket _udpServerSocket;
            std::map<std::string, std::function<std::pair<std::string, CnpStatus>(const std::string&)>> _methodsMap;
            std::atomic<bool> _needStopServer;

        public:
            UdpServer(const char* serverIp, int32_t port);

            virtual ~UdpServer() = default;

            void initializeServer();

            void startServer();

            void stopServer();

            void waitInComingRequests();

        private:
            static int clientMethod(UdpServer *thisPtr, struct sockaddr_in &&targetAddress, uint32_t firstMessageLen);

            uint32_t readRequestLength(struct sockaddr_in &clientSocket);

            std::shared_ptr<CnpRequest> readRequest(const struct sockaddr_in &targetAddress, uint32_t messageLen);

            std::shared_ptr<CnpResponse> getResponse(const std::shared_ptr<CnpRequest>& request);
        };
    }
}

#endif //COMMUTATOR_UDPSERVER_H
