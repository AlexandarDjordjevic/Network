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

void socketDisconnectHandler(int socketFd, uint32_t reson)
{
    std::cout << "Socket " << socketFd << " disconnected. Reason: " << reson << std::endl;
}

void serverTask()
{
    server.create(Network::Socket::Domain::D_INET,
                  Network::Socket::Type::SOCK_STREAM);
    server.bind(1234);
    server.listen();
    while (true)
    {
        if (server.accept(&client))
            eventManager.addSocketForMonitoring(client);
    }
}

void eventManagerTask()
{
    eventManager.create();
    eventManager.setDataReceivedDelegate(dataReceivedHandler);
    eventManager.setDisconnectDelegate(socketDisconnectHandler);
    std::cout << "Entering event loop" << std::endl;
    while (true)
    {
        eventManager.eventLoop();
    }
}

void callMeOnDataReceived(uint8_t *data, size_t length)
{
    std::cout << "Received length: " << length << " Received message: " << data << std::endl;
}

#include <thread>

int main()
{

    // std::thread aceptor(serverTask);
    // std::thread eventManger(eventManagerTask);

    // aceptor.join();
    // eventor.join();

    // client.close();
    // Network::Stream::Client client;

    std::string ip_address = "192.168.100.50";
    uint16_t port = 1234;

    // if (client.connect(ip_address, port))
    // {
    //     std::cout << "Connected to " << ip_address << ":" << port << std::endl;
    // }
    // else
    // {
    //     std::cout << "Fail to connect" << std::endl;
    // }

    Network::Datagram::Server udp_server;
    udp_server.setDataReceiveDelegate(callMeOnDataReceived);
    if (udp_server.startReceive(1234))
    {
        std::cout << "Listening on port 1234 for incoming data." << std::endl;
    }
    udp_server.run();

    uint16_t finish;
    std::cin >> finish;
    // if (udp_server.l)
    // {
    //     std::cout << "Success!" << std::endl;
    // }
    // else
    // {
    //     std::cout << "Fail to write datagram!" << std::endl;
    // }

    // uint8_t message_buffer[1024] = {0};

    // auto readed_length = client.read(message_buffer, sizeof(message_buffer));

    // std::cout << "Client got " << readed_length << " bytes. Message :" << message_buffer << std::endl;

    // client.disconnect();

    std::cout
        << "Done" << std::endl;
    return 0;
}