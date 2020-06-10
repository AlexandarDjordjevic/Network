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

        void Server::enqueClient(const std::shared_ptr<Client> &client)
        {
            std::unique_lock<std::mutex> lock(pimpl->clientListMutex);
            pimpl->clients.push_back(client);
        }

        void Server::accept()
        {
            pimpl->run = true;
            std::cout << "Server is running at: "
                      << pimpl->serverSocket.getIp() << ":"
                      << pimpl->serverSocket.getPort() << std::endl;
            while (pimpl->run)
            {
                auto newClient = std::make_shared<Client>();
                if (pimpl->serverSocket.accept(newClient->getSocket()) == true)
                {
                    std::cout << "Adding new clinet to the clinets list" << std::endl;
                    enqueClient(newClient);
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
        }

        //void(*disconnectDelegate_t)(int fd, uint32_t reason);
        bool Server::eventManager()
        {
            if (pimpl->socketEventLoop.create() == false)
                return false;
            pimpl->socketEventLoop.setDataReceivedDelegate(dataReceived);
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
    } // namespace Stream

} // namespace Network
