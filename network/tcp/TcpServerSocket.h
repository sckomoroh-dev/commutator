//
// Created by dev on 4/24/18.
//

#ifndef COMMUTATOR_TCPSERVERSOCKET_H
#define COMMUTATOR_TCPSERVERSOCKET_H

#include <memory>
#include <cstring>
#include "../SocketException.h"
#include "TcpClientSocket.h"

class TcpServerSocket
{
private:
    int32_t _socket;
    struct sockaddr_in _targetSocketAddress;

public:
    TcpServerSocket(const char* serverIp, uint16_t port)
        : _socket(-1)
        , _targetSocketAddress { 0 }
    {
        _targetSocketAddress.sin_family = AF_INET;
        _targetSocketAddress.sin_port = htons(port);
        _targetSocketAddress.sin_addr.s_addr = inet_addr(serverIp);
    }

    void createSocket()
    {
        _socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (_socket == -1)
        {
            throw SocketException("Cannot to create the socket", errno);
        }
    }

    void close()
    {
        if (_socket != -1)
        {
            ::close(_socket);

            _socket = -1;
        }
    }

    void bind()
    {
        if (::bind(_socket,
                reinterpret_cast<struct sockaddr*>(&_targetSocketAddress),
                sizeof(_targetSocketAddress)) < 0)
        {
            throw SocketException("Unable to bind server socket", errno);
        }
    }

    void listen()
    {
        if (::listen(_socket, 1) == -1)
        {
            throw SocketException("Unable to start listen", errno);
        }
    }

    std::shared_ptr<TcpClientSocket> accept()
    {
        struct sockaddr_in clientSocketAddress = { 0 };
        auto clientSocketAddressSize = sizeof(clientSocketAddress);

        auto clientSocketDescriptor = ::accept(_socket,
                reinterpret_cast<struct sockaddr*>(&clientSocketAddress),
                reinterpret_cast<socklen_t*>(&clientSocketAddressSize));

        if (clientSocketDescriptor == -1)
        {
            throw SocketException("Unable to accept client socket", errno);
        }

        return std::make_shared<TcpClientSocket>(clientSocketDescriptor, clientSocketAddress);
    }
};

#endif //COMMUTATOR_TCPSERVERSOCKET_H
