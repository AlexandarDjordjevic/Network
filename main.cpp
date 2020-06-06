#include <iostream>

#include <Network/Stream/Server.hpp>

int main(){
    Network::Stream::Server server;
    if(server.listen("192.168.0.108", 2000)){
        server.accept();
    } 
    std::cout << "Server error: " << server.getLastError() << std::endl;
    return 0;
}