//
// Created by dev on 5/3/18.
//

#ifndef COMMUTATOR_UDPSERVER_H
#define COMMUTATOR_UDPSERVER_H

#include <memory>
#include <atomic>
#include <thread>
#include <future>
#include <list>
#include <map>
#include "../../../sockets/udp/UdpServerSocket.h"
#include "../../CnpProtocol.h"


namespace server
{
    namespace udp
    {
        class UdpServer : protected CnpProtocol
        {
        private:
            network::sockets::udp::UdpServerSocket _udpServerSocket;
            std::map<std::string, std::function<std::pair<std::string, std::string>(std::string)>> _methodsMap;
            std::atomic<bool> _needStopServer;

        public:
            UdpServer(const char* serverIp, int32_t port);

            virtual ~UdpServer() = default;

            void initializeServer();

            void startServer();

            void stopServer();

            void waitInComingRequests();

        private:
            static void clientMethod(UdpServer *thisPtr, struct sockaddr_in &&targetAddress, uint32_t firstMessageLen);

            uint32_t readRequestMessageLength(struct sockaddr_in &clientSocket);

            char* readRequestMessage(const struct sockaddr_in &targetAddress, uint32_t messageLen);

            std::string buildResponseString(std::map<std::string, std::string> requestParameters);
        };
    }
}

#endif //COMMUTATOR_UDPSERVER_H
