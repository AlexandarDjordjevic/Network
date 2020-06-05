#include <Network/Stream/Server.hpp>

namespace Network
{
    namespace Stream
    {
        Server::Server(){

        }
        Server::~Server(){
            
        }

        void Server::setReceiveHandler(receiveHandler_t handler){
            receiveHandler = handler;
        }
    } // namespace Stream
    
} // namespace Network
