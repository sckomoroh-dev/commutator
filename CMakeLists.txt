cmake_minimum_required(VERSION 3.9)
project(commutator)

set(CMAKE_CXX_STANDARD 14)

add_executable(commutator
        main.cpp
        network/sockets/tcp/TcpClientSocket.h
        network/sockets/tcp/TcpServerSocket.h
        network/sockets/SocketException.h
        network/sockets/tcp/TcpSocket.h
        network/sockets/udp/UdpSocket.h
        network/sockets/udp/UdpClientSocket.h
        network/sockets/udp/UdpServerSocket.h
        network/sockets/Socket.h
        network/cnp/server/udp/UdpServer.h
        network/cnp/client/udp/UdpClient.h
        network/sockets/Socket.cpp
        network/sockets/udp/UdpSocket.cpp
        network/sockets/udp/UdpServerSocket.cpp
        network/sockets/udp/UdpClientSocket.cpp
        network/sockets/SocketException.cpp
        network/sockets/tcp/TcpSocket.cpp
        network/sockets/tcp/TcpServerSocket.cpp
        network/sockets/tcp/TcpClientSocket.cpp
        network/cnp/server/ServerException.cpp
        network/cnp/server/ServerException.h
        network/cnp/server/udp/UdpServer.cpp
        network/cnp/message/CnpStatus.h
        network/cnp/client/udp/UdpClient.cpp network/cnp/message/CnpRequest.cpp network/cnp/message/CnpRequest.h network/cnp/message/CnpResponse.cpp network/cnp/message/CnpResponse.h network/cnp/message/CnpVersions.h network/cnp/CnpException.cpp network/cnp/CnpException.h network/cnp/message/CnpMessage.cpp network/cnp/message/CnpMessage.h)

target_link_libraries(commutator pthread)