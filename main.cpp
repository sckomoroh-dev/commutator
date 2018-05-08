#include <cstdio>
#include <thread>
#include <future>

#include <stdlib.h>

#include "network/cnp/server/udp/UdpServer.h"
#include "network/cnp/client/udp/UdpClient.h"

using namespace network::sockets;

std::atomic<bool> g_init(false);

template <typename F, typename... Ts>
inline auto reallyAsync(F&& f, Ts&&... params)
{
    return std::async(std::launch::async, std::forward<F>(f), std::forward<Ts>(params)...);
}

void serverThread()
{
    printf ("Creating server\n");
    network::cnp::server::udp::UdpServer udpServer("127.0.0.1", 8090);

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

    for (auto i=0; i<100; i++)
    {
        auto asyncRes = reallyAsync([i]()
        {
            printf("");
            network::cnp::client::udp::UdpClient udpClient("127.0.0.1", 8090);
            char buffer[32] = { 0 };
            sprintf(buffer, "%i %i", std::this_thread::get_id(), i);

            auto request = network::cnp::message::CnpRequest::create(network::cnp::message::CnpVersion::Version10, "Echo", buffer);
            printf("Request: \n=============\n%s\n=============\n\n", request->data().c_str());

            udpClient.sendRequest(request);

            auto response = udpClient.readResponse();

            printf("Response:\n-------------\n%s\n-------------\n\n", response->data().c_str());
        });

        asyncRes.get();
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