#include <Network/Stream/Server.hpp>

#include <sys/epoll.h>
namespace Network
{
    namespace Stream
    {
        Server::Server()
            : error(Error::NO_ERROR)
        {
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
            if (serverSocket.create(
                    Socket::Domain::D_INET,
                    Socket::Type::SOCK_STREAM) == false)
            {
                error = Error::CREATE_SOCKET_ERROR;
                return false;
            }
            if (serverSocket.bind(address, port) == false)
            {
                error = Error::BIND_SOCKET_ERROR;
                return false;
            }
            if (serverSocket.listen() == false)
            {
                error = Error::LISTEN_SOCKET_ERROR;
                return false;
            }
            return true;
        }

        void Server::enqueClient(const std::shared_ptr<Socket> client)
        {
            std::unique_lock<std::mutex> lock(clientListMutex);
            clients.push_back(client);
        }

        void Server::accept()
        {
            run = true;
            std::cout << "Server is running at: "
                      << serverSocket.getIp() << ":"
                      << serverSocket.getPort() << std::endl;
            while (run)
            {
                auto newClient = std::make_shared<Socket>();
                if (serverSocket.accept(newClient.get()) == true)
                {
                    std::cout << "Adding new clinet to the clinets list" << std::endl;
                    clients.push_back(newClient);
                }
                else
                {
                    std::cout << "Fail to accept new client" << std::endl;
                }
            }
        }

        void Server::eventManager()
        {
        }

        std::string Server::getLastError()
        {
            switch (error)
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

        void Server::setReceivedDataDelegate(readDataDelegate_t handler)
        {
            receiveHandler = handler;
        }
    } // namespace Stream

} // namespace Network
