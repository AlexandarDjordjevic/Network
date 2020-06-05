#include <iostream>

#include <Network/Socket.hpp>

int main(){
    Network::Socket socket(
        Network::Socket::Domain::D_INET, 
        Network::Socket::Type::SOCK_STREAM
    );

    if(!socket.create()){
        std::cerr << "Fail to create socket" << std::endl;
        exit(-1);
    }

    if(!socket.bind(6000)){
        std::cerr << "Fail to bind socket" << std::endl;
        exit(-1);
    }
    if(!socket.listen()){
        std::cerr << "Fail to listen on socket" << std::endl;
        exit(-1);
    }
    
    Network::Socket client;
    std::cout <<"Waiting for a clients ..." << std::endl;
    if(socket.accept(client)){
        std::cout << "Client " << client.getIp() << " connected" << std::endl;
        uint8_t buffer[1024];
        client.read(buffer, sizeof(buffer));
        client.write(std::string("Hello From Server!"));
    }else{
        std::cerr << "Accepting client failed" << std::endl;
    }
    socket.shutdown();
    socket.close();
    return 0;
}