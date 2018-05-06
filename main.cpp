#include <cstdio>
#include <thread>
#include "network/cnp/server/udp/UdpServer.h"
#include "network/cnp/client/udp/UdpClient.h"
#include "network/cnp/server/ServerCommandConstants.h"

using namespace network::sockets;

std::atomic<bool> g_init(false);

void serverThread()
{
    printf ("Creating server\n");
    server::udp::UdpServer udpServer("127.0.0.1", 8090);

    printf("Initializing the server\n");
    udpServer.initializeServer();

    printf("Start server\n");
    udpServer.startServer();

    printf("Set server init TRUE\n");
    g_init = true;

    printf("Wait incoming connections\n");
    udpServer.waitInComingRequests();
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
        udpClient.sendMessage(QUERY_SERVER_VERSION);
        auto response = udpClient.readResponse();

        printf("Response:\n-------------\n%s\n-------------\n\n", response.c_str());
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