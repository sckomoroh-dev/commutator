cmake_minimum_required(VERSION 3.10)
project(commutator)

set(CMAKE_CXX_STANDARD 11)

add_executable(commutator main.cpp network/tcp/TcpClientSocket.h network/tcp/TcpServerSocket.h network/SocketException.h network/tcp/TcpSocket.h network/udp/UdpSocket.h network/udp/UdpClientSocket.h network/udp/UdpServerSocket.h network/Socket.h)

target_link_libraries(commutator pthread)