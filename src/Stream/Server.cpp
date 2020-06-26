#include <Network/Stream/Server.hpp>
#include <Network/SocketEventManager.hpp>

#include <algorithm>
#include <functional>
namespace Network
{
    namespace Stream
    {
        struct Server::impl
        {
            std::vector<std::shared_ptr<Client>> clients;
            Socket serverSocket;
            SocketEventManager::dataReceiveDelegate_t dataReceiveHandler;
            SocketEventManager::disconnectDelegate_t clientDisconnectedHandler;
            Error error;
            bool run;
            std::mutex clientListMutex;
            SocketEventManager socketEventLoop;
        };
        
        Server::Server()
            : pimpl(new Server::impl)
        {
            pimpl->error = Error::NO_ERROR;
        }

        Server::~Server()
        {
        }

        bool Server::listen(uint16_t port)
        {
            listen(Network::Socket::ANY_ADDRES, port);
        }

        bool Server::listen(std::string address, uint16_t port)
        {
            if (pimpl->serverSocket.create(
                    Socket::Domain::D_INET,
                    Socket::Type::SOCK_STREAM) == false)
            {
                pimpl->error = Error::CREATE_SOCKET_ERROR;
                return false;
            }
            if (pimpl->serverSocket.bind(address, port) == false)
            {
                pimpl->error = Error::BIND_SOCKET_ERROR;
                return false;
            }
            if (pimpl->serverSocket.listen() == false)
            {
                pimpl->error = Error::LISTEN_SOCKET_ERROR;
                return false;
            }
            return true;
        }

        void Server::enqueueClient(const std::shared_ptr<Client> &client)
        {
            std::unique_lock<std::mutex> lock(pimpl->clientListMutex);
            pimpl->clients.push_back(client);
            pimpl->socketEventLoop.addSocketForMonitoring(*(client->getSocket()));
        }

        void Server::accept()
        {
            pimpl->run = true;
            std::cout << "[Server] Running at: "
                      << pimpl->serverSocket.getIp() << ":"
                      << pimpl->serverSocket.getPort() << std::endl;
            while (pimpl->run)
            {
                auto newClient = std::make_shared<Client>();
                if (pimpl->serverSocket.accept(newClient->getSocket()) == true)
                {
                    std::cout << "[Server] New Client Connected! IP: " << newClient->getSocket()->getIp() << std::endl;
                    enqueueClient(newClient);
                }
                else
                {
                    std::cout << "Fail to accept new client" << std::endl;
                }
            }
        }

        void Server::dataReceived(int socket_fd, uint8_t *data, size_t len)
        {
            auto client = std::find_if(
                std::begin(pimpl->clients),
                std::end(pimpl->clients),
                [socket_fd](std::shared_ptr<Client> client) {
                    return socket_fd == client->getSocket()->getDescriptor();
                });
            std::cout << "[Server] Data received: " << data << std::endl;
        }

        void Server::clientDisconnected(int socket_fd, uint32_t reason){
            /* Suppress warrning */
            (void)reason;
            for(auto client = pimpl->clients.begin(); client < pimpl->clients.end(); ++client){
                if ((*client)->getSocket()->getDescriptor() == socket_fd){
                    std::cout << "[Server] Client Disconnected. IP: " 
                              << (*client)->getSocket()->getIp() 
                              << std::endl;
                    pimpl->socketEventLoop.removeSocketFromMonitoring(socket_fd);
                    pimpl->clients.erase(client);
                    return;
                }
            }       
        }

        bool Server::eventManager()
        {
            if (pimpl->socketEventLoop.create() == false)
                return false;

            /* Set data received callback */
            auto dataCb = std::bind(&Server::dataReceived, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
            pimpl->socketEventLoop.setDataReceivedDelegate(dataCb);

            /* Set client disconnect callback */
            auto discCb = std::bind(&Server::clientDisconnected, this, std::placeholders::_1, std::placeholders::_2);
            pimpl->socketEventLoop.setDisconnectDelegate(discCb);

            /* Start event loop */
            pimpl->socketEventLoop.eventLoop();
        }

        std::string Server::getLastError()
        {
            switch (pimpl->error)
            {
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
            default:
                return "Unknown error";
            }
        }

        void Server::setReceivedDataDelegate(SocketEventManager::dataReceiveDelegate_t handler)
        {
            pimpl->dataReceiveHandler = handler;
        }

        void Server::setClientDisconnectedDelegate(SocketEventManager::disconnectDelegate_t handler){
            pimpl->clientDisconnectedHandler = handler;
        }
    } // namespace Stream

} // namespace Network
