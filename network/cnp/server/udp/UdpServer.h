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


namespace network
{
    namespace cnp
    {
        namespace server
        {
            namespace udp
            {
                class UdpServer
                {
                private:
                    network::sockets::udp::UdpServerSocket _udpServerSocket;
                    std::map<std::string, std::function<std::pair<const std::string, message::CnpStatus>(const std::string &)>> _methodsMap;
                    std::atomic<bool> _needStopServer;

                public:
                    UdpServer(const char *serverIp, int32_t port);

                    virtual ~UdpServer() = default;

                    void initializeServer();

                    void startServer();

                    void stopServer();

                    void waitInComingRequests();

                private:
                    static void clientMethod(UdpServer *thisPtr, struct sockaddr_in &&targetAddress, uint32_t&& firstMessageLen);

                    uint32_t readRequestLength(struct sockaddr_in &clientSocket);

                    std::shared_ptr<message::CnpRequest>
                    readRequest(const struct sockaddr_in &targetAddress, uint32_t messageLen);

                    std::shared_ptr<message::CnpResponse> getResponse(const std::shared_ptr<message::CnpRequest> &request);

                    template<typename TResponse>
                    void sendResponse(TResponse&& response, struct sockaddr_in&& targetAddress)
                    {
                        auto responseString = response->toString();

                        uint32_t responseLength = responseString.length();
                        _udpServerSocket.sendBuffer(static_cast<void*>(&responseLength), sizeof(uint32_t), targetAddress);
                        _udpServerSocket.sendBuffer(static_cast<void*>(const_cast<char*>(responseString.c_str())), responseLength, targetAddress);
                    }

                    template <typename F, typename... Ts>
                    inline auto reallyAsync(F&& f, Ts&&... params)
                    {
                        return std::async(std::launch::async, std::forward<F>(f), std::forward<Ts>(params)...);
                    }
                };
            }
        }
    }
}

#endif //COMMUTATOR_UDPSERVER_H
