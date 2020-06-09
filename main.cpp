#include <iostream>

#include <Network/Stream/Server.hpp>
#include <Network/Socket.hpp>
#include <Network/SocketEventManager.hpp>
#include <Network/Stream/Client.hpp>
#include <Network/Datagram/Client.hpp>
#include <Network/Datagram/Server.hpp>

Network::SocketEventManager eventManager;
Network::Socket server, client;

void dataReceivedHandler(int sockFd, uint8_t *buffer, size_t len)
{
    std::cout << "Message received. Len: " << len << " Data: " << buffer << std::endl;
}

void socketDisconnectHandler(int socketFd, uint32_t reason)
{
    std::cout << "Socket " << socketFd << " disconnected. Reason: " << reason << std::endl;
}


#include <thread>

int main()
{
    Network::Stream::Server server;
    server.listen(1234);

    std::thread aceptor(&Network::Stream::Server::accept, &server);
    std::thread eventManger(&Network::Stream::Server::eventManager, &server);

    aceptor.join();
    eventManger.join();

    std::cout
        << "Done" << std::endl;
    return 0;
}