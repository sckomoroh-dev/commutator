#include <cstdio>
#include <thread>
#include "network/server/udp/UdpServer.h"
#include "network/client/udp/UdpClient.h"

using namespace network::sockets;

std::atomic<bool> g_init(false);

void serverThread()
{
    printf ("Creating server\n");
    server::udp::UdpServer udpServer("127.0.0.1", 8090);

    printf("Start server\n");
    udpServer.startServer();

    printf("Set server init TRUE\n");
    g_init = true;

    printf("Wait incoming connections\n");
    udpServer.waitIncommingRequests();
}

void clientThread()
{
    while (g_init == false)
    {
        sleep(1);
    }

    client::udp::UdpClient udpClient("127.0.0.1", 8090);

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

    printf("End\n");

    return 0;
}