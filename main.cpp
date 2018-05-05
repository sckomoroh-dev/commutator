#include <cstdio>
#include <thread>
#include "network/tcp/TcpServerSocket.h"
#include "network/udp/UdpServerSocket.h"
#include "server/udp/UdpServer.h"
#include "client/udp/UdpClient.h"

using namespace network::sockets;

void tcpServerThreadFunc()
{
    char buffer[1024] = { 0 };
    strcpy(buffer, "Network message");

    printf("[SERVER] Start\n");
    tcp::TcpServerSocket serverSocket("127.0.0.1", 8008);

    printf("[SERVER] Bind socket\n");
    serverSocket.bind();

    printf("[SERVER] Listen\n");
    serverSocket.listen();

    printf("[SERVER] Accept incoming connections\n");
    auto clientSocket = serverSocket.accept();

    printf("[SERVER] Send buffer\n");
    clientSocket->sendBuffer(static_cast<void*>(buffer), strlen(buffer) + 1024);

    auto response = malloc(1024);
    memset(response, 0, 1024);

    printf("[SERVER] Read buffer\n");
    clientSocket->readBuffer(response, 1024);

    printf("Client send: %s\n", buffer);

    printf("[SERVER] Close client socket\n");
    clientSocket->close();

    printf("[SERVER] Close server socket\n");
    serverSocket.close();
}

void tcpClientThreadFunc()
{
    char buffer[1024] = { 0 };

    printf("[client] Start\n");
    tcp::TcpClientSocket clientSocket("127.0.0.1", 8008);

    printf("[client] Connect\n");
    clientSocket.connect();

    printf("[client] read buffer\n");
    clientSocket.readBuffer(static_cast<void*>(buffer), 1024);

    printf("[client] Server send: %s\n", buffer);

    memset(static_cast<void*>(buffer), 0, 1024);
    strcpy(buffer, "This is message for server");

    printf("[client] send buffer\n");
    clientSocket.sendBuffer(static_cast<void*>(buffer), 1024);

    printf("[client] Closing\n");
    clientSocket.close();
}

void udpServerThreadFunc()
{
    char buffer[1024] = { 0 };
    printf("[SERVER] Start\n");
    udp::UdpServerSocket serverSocket("127.0.0.1", 8010);

    printf("[SERVER] Bind\n");
    serverSocket.bind();

    printf("[SERVER] Read from client\n");
    auto clientSocket = serverSocket.readBuffer(static_cast<void*>(buffer), 1024);

    printf("[SERVER] Received message: %s\n", buffer);

    memset(buffer, 0, 1024);
    strcpy(buffer, "Server response to client");

    printf("[SERVER] Send to client\n");
    clientSocket->sendBuffer(static_cast<void*>(buffer), 1024);

    printf("[SERVER] Close socket\n");
    serverSocket.close();
}

void udpClientThreadFunc()
{
    char buffer[1024] = { 0 };
    strcpy(buffer, "Network message");

    printf("[client] Start\n");
    udp::UdpClientSocket clientSocket("127.0.0.1", 8010);

    printf("[client] Send to server\n");
    clientSocket.sendBuffer(static_cast<void*>(buffer), 1024);

    memset(buffer, 0, 1024);

    printf("[client] Read from server\n");
    clientSocket.readBuffer(static_cast<void*>(buffer), 1024);

    printf("[client] Received message: %s\n", buffer);

    printf("[client] Close socket\n");
    clientSocket.close();
}

auto g_init = false;

void serverThread()
{
    server::udp::UdpServer udpServer("127.0.0.1", 8090);
    udpServer.startServer();

    g_init = true;

    udpServer.waitIncommingRequests();
}

void clientThread()
{
    while (g_init == false)
    {
        sleep(1);
    }

    UdpClient udpClient("127.0.0.1", 8090);

    while (true)
    {
        udpClient.sendHelloMessage();
    }
}

int main()
{
    printf("Start\n");

    std::thread server(serverThread);
    std::thread client(clientThread);

    server.join();
    client.join();

    //server::udp::UdpServer udpServer("127.0.0.1", 8090);
    /*{
        printf("TCP section\n");
        std::thread serverThread(tcpServerThreadFunc);
        std::thread clientThread(tcpClientThreadFunc);
        serverThread.join();
        clientThread.join();
    }

    {
        printf("UDP section\n");
        std::thread serverThread(udpServerThreadFunc);
        std::thread clientThread(udpClientThreadFunc);
        serverThread.join();
        clientThread.join();
    }*/

    printf("End\n");

    return 0;
}