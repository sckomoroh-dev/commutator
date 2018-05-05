//
// Created by dev on 5/3/18.
//

#ifndef COMMUTATOR_UDPCLIENT_H
#define COMMUTATOR_UDPCLIENT_H


#include <cstring>
#include "../../network/udp/UdpClientSocket.h"

class UdpClient
{
private:
    network::sockets::udp::UdpClientSocket _udpClientSocket;

public:
    template<typename TServerIp, typename TPort>
    UdpClient(TServerIp &&serverIp, TPort &&port)
        : _udpClientSocket(serverIp, port)
    {
    }

    void sendHelloMessage()
    {
        char buffer[1024] = { 0 };
        strcpy(buffer, buildMessage("Hello server"));
        _udpClientSocket.sendBuffer(static_cast<void*>(buffer), strlen(buffer));
    }

    template<typename TMessage>
    char* buildMessage(TMessage&& message)
    {
        char buffer[1024] = { 0 };
        sprintf(buffer, "CNP v1.0\nMessage: %s\n\n", message);

        return std::move(buffer);
    }
};

#endif //COMMUTATOR_UDPCLIENT_H
