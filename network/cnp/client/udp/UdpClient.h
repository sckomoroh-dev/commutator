//
// Created by dev on 5/3/18.
//

#ifndef COMMUTATOR_UDPCLIENT_H
#define COMMUTATOR_UDPCLIENT_H


#include <cstring>
#include "../../../sockets/udp/UdpClientSocket.h"
#include "../../CnpProtocol.h"

namespace client
{
    namespace udp
    {
        class UdpClient : protected CnpProtocol
        {
        private:
            network::sockets::udp::UdpClientSocket _udpClientSocket;

        public:
            UdpClient(const char* serverIp, int32_t port);

            void sendMessage(std::string& command);

            void sendMessage(std::string&& command);

            const std::string readResponse();
        };
    }
}

#endif //COMMUTATOR_UDPCLIENT_H
