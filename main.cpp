#include <iostream>

#include <Network/Stream/Server.hpp>

int main(){
    Network::Stream::Server server;
    server.listen(3000);
    std::cout << "Server error: " << server.getLastError() << std::endl;
    return 0;
}