#include <Network/Datagram/Server.hpp>
#include <Network/Socket.hpp>

namespace Network
{
    namespace Datagram
    {
        struct Server::impl
        {
            uint16_t port;
            Socket socket;
            dataRecieveDelegate_t dataReceiveDelegate;
        };

        Server::Server()
            : pimpl(new Server::impl)
        {
        }

        Server::~Server()
        {
        }

        bool Server::startReceive(uint16_t port)
        {
            if (false == pimpl->socket.create(
                             Network::Socket::Domain::D_INET,
                             Network::Socket::Type::SOCK_DGRAM))
            {
                std::perror("Datagram create socket");
                return false;
            }

            if (false == pimpl->socket.bind(port))
            {
                std::perror("Datagram bind socket");
                pimpl->socket.close();
                return false;
            }

            return true;
        }

        void Server::run()
        {
            while (true)
            {
                uint8_t buffer[1024];
                Socket client;
                auto rec_len = pimpl->socket.receiveFrom(buffer, sizeof(buffer), &client);
                if (pimpl->dataReceiveDelegate)
                    pimpl->dataReceiveDelegate(buffer, rec_len);
            }
        }

        void Server::setDataReceiveDelegate(dataRecieveDelegate_t receiveDelegate)
        {
            pimpl->dataReceiveDelegate = receiveDelegate;
        }

    } // namespace Datagram

} // namespace Network
