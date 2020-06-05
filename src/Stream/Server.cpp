#include <Network/Stream/Server.hpp>

#include <iostream>
namespace Network
{
    namespace Stream
    {
        Server::Server(){

        }
        Server::~Server(){
            
        }

        bool Server::listen(uint16_t port){
            std::shared_ptr<Socket> newClient = 
                                    std::make_shared<Socket>();
            if (serverSocket.create(
                Network::Socket::Domain::D_INET,
                Network::Socket::Type::SOCK_STREAM
            ) == false) {
                error = Error::CREATE_SOCKET_ERROR;
                return false;
            }
            if (serverSocket.bind(port) == false) {
                error = Error::BIND_SOCKET_ERROR;
                return false;
            }
            if (serverSocket.listen() == false) {
                error = Error::LISTEN_SOCKET_ERROR;
                return false;
            }
            if (serverSocket.accept(newClient.get()) == true){
                std::cout << "Adding new clinet to the clinets list" << std::endl;
                clients.push_back(newClient);     
            }else{
                std::cout << "Fail to accept new client" << std::endl;
            }
           
        }

        bool Server::listen(std::string address, uint16_t port){
            if (serverSocket.create() == false) return false;
            if (serverSocket.bind(port) == false) return false;

        }

        std::string Server::getLastError(){
            switch(error){
                case Error::NO_ERROR:
                    return "No error";
                case Error::CREATE_SOCKET_ERROR:
                    return "Fail to create socket";
                case Error::BIND_SOCKET_ERROR:
                    return "Fail to bind socket";
                case Error::LISTEN_SOCKET_ERROR:
                    return "Fail to listen on socket";
                case Error::ACCEPT_SOCKET_ERROR:
                    return "Fail to accept on socket";
            }
        }

        void Server::setReceivedDataDelegate(readDataDelegate_t handler){
            receiveHandler = handler;
        }
    } // namespace Stream
    
} // namespace Network
