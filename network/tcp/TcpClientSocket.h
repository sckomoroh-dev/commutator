//
// Created by dev on 4/24/18.
//

#ifndef COMMUTATOR_TCPCLIENTSOCKET_H
#define COMMUTATOR_TCPCLIENTSOCKET_H

#include <cstddef>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "../SocketException.h"

class TcpClientSocket
{
private:
    int32_t _socket;
    struct sockaddr_in _targetSocketAddress;

public:
    TcpClientSocket(const char* serverIp, uint16_t port)
        : _socket(-1)
        , _targetSocketAddress { 0 }
    {
        _targetSocketAddress.sin_family = AF_INET;
        _targetSocketAddress.sin_port = htons(port);
        _targetSocketAddress.sin_addr.s_addr = inet_addr(serverIp);
    }

    TcpClientSocket(int32_t socketDescriptor, struct sockaddr_in socketAddress)
        : _socket(socketDescriptor)
        , _targetSocketAddress(socketAddress)
    {
        printf("Creating client socket from server side\n");
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

    void connect()
    {
        if (::connect(_socket,
                      reinterpret_cast<struct sockaddr*>(&_targetSocketAddress),
                      sizeof(_targetSocketAddress)) == -1)
        {
            throw SocketException("Unable to connect to server socket", errno);
        }
    }

    void sendBuffer(void* buffer, size_t bufferSize)
    {
        if (send(_socket,
                 buffer,
                 bufferSize,
                 MSG_WAITALL) < 1)
        {
            throw SocketException("Unable to send data to socket", errno);
        }
    }

    void readBuffer(void* buffer, size_t bufferSize)
    {
        if (recv(_socket,
                 buffer,
                 bufferSize,
                 MSG_WAITALL) < 1)
        {
            throw SocketException("Unable to receive data from socket", errno);
        }
    }
};

#endif //COMMUTATOR_TCPCLIENTSOCKET_H
