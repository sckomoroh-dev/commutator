#include <cstdio>
#include <thread>
#include <future>
#include <sstream>

#include "network/cnp/tcp/TcpServer.h"
#include "network/cnp/tcp/TcpClient.h"

std::atomic<bool> g_init(false);

void serverThread()
{
    printf ("Creating server\n");
    network::cnp::tcp::TcpServer server("127.0.0.1", 8090);

    printf("Initializing the server\n");
    server.initializeServer();

    printf("Start server\n");
    server.startServer();

    printf("Set server init TRUE\n");
    g_init = true;

    printf("Wait incoming connections\n");
    server.waitInComingRequests();
}

void clientThread()
{
    while (!g_init)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    for (auto i=0; i<1; i++)
    {
        auto thread = std::thread([i]()
        {
            printf("");
            network::cnp::tcp::TcpClient client("127.0.0.1", 8090);
			client.connect();

			std::stringstream stringStream;
			stringStream << std::this_thread::get_id();
			stringStream << i;

            auto request = network::cnp::message::CnpRequest::create(network::cnp::message::CnpVersion::Version10, "Echo", stringStream.str());

            client.sendRequest(request);

            auto response = client.readResponse();

            printf("Response:\n-------------\n%s\n-------------\n\n", response->data().c_str());
        });

        thread.detach();
    }
}

int main()
{
	platform_init_sockets();

    std::thread server(serverThread);
    std::thread client(clientThread);

    server.join();
    client.join();

    printf("End\n");

	std::this_thread::sleep_for(std::chrono::seconds(60));

	platform_cleanup_sockets();

    return 0;
}