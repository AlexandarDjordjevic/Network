#include <iostream>

#include <Network/Stream/Server.hpp>
#include <Network/Socket.hpp>
#include <Network/SocketEventManager.hpp>

Network::SocketEventManager eventManager;
Network::Socket server, client;

void dataReceivedHandler(int sockFd, uint8_t* buffer, size_t len){
    std::cout << "Message received. Len: " << len << " Data: " << buffer << std::endl;
}

void socketDisconnectHandler(int socketFd, uint32_t reson){
    std::cout << "Socket " << socketFd << " disconnected. Reason: " << reson << std::endl;
}

void serverTask(){
    server.create(Network::Socket::Domain::D_INET,
                Network::Socket::Type::SOCK_STREAM
                );
    server.bind(1234);
    server.listen();
    while(true){
        if(server.accept(&client))
            eventManager.addSocketForMonitoring(client);
    }
}

void eventManagerTask(){
    eventManager.create();
    eventManager.setDataReceivedDelegate(dataReceivedHandler);
    eventManager.setDisconnectDelegate(socketDisconnectHandler);
    std::cout << "Entering event loop" << std::endl;
    while (true){
        eventManager.eventLoop();
    }
}

#include <thread>

int main(){

    std::thread aceptor(serverTask);
    std::thread eventor(eventManagerTask);

    aceptor.join();
    eventor.join();

  
    client.close();
    std::cout << "Done" << std::endl;
    return 0;
}