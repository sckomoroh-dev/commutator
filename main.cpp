#include <cstdio>
#include <thread>
#include "network/tcp/TcpServerSocket.h"

void serverThreadFunc()
{
    char buffer[1024] = { 0 };
    strcpy(buffer, "Network message");

    printf("[SERVER] Start\n");
    TcpServerSocket serverSocket("127.0.0.1", 8008);

    printf("[SERVER] Create socket\n");
    serverSocket.createSocket();

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

void clientThreadFunc()
{
    char buffer[1024] = { 0 };

    printf("[client] Start\n");
    TcpClientSocket clientSocket("127.0.0.1", 8008);

    printf("[client] Create socket\n");
    clientSocket.createSocket();

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

int main()
{
    printf("Start\n");

    {
        std::thread serverThread(serverThreadFunc);
        std::thread clientThread(clientThreadFunc);
        serverThread.join();
        clientThread.join();
    }

    printf("End\n");

    return 0;
}