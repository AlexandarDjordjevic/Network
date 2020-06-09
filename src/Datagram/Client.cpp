#include <Network/Datagram/Client.hpp>
#include <Network/Socket.hpp>

namespace Network
{
    namespace Datagram
    {
        struct Client::impl
        {
            Socket socket;
        };

        Client::Client()
            : pimpl(new Client::impl){

              };
        Client::~Client() = default;

        bool Client::init()
        {
            if (false == pimpl->socket.create(
                             Network::Socket::Domain::D_INET,
                             Network::Socket::Type::SOCK_DGRAM))
            {
                std::perror("Datagram socket create");
                pimpl->socket.close();
                return false;
            }
        }

        bool Client::write(const std::string &ip_address, uint16_t port, const char *message, size_t length)
        {
            struct sockaddr_in sock_addr;
            inet_pton(int(Network::Socket::Domain::D_INET), ip_address.c_str(), &(sock_addr.sin_addr.s_addr));
            sock_addr.sin_family = sa_family_t(Network::Socket::Domain::D_INET);
            sock_addr.sin_port = htons(port);

            if (-1 == sendto(
                          pimpl->socket.getDescriptor(),
                          message,
                          length,
                          0,
                          (sockaddr *)&sock_addr,
                          sizeof(sock_addr)))
            {
                std::perror("Sendto error");
                return false;
            }
            return true;
        }

        bool Client::write(const std::string &ip_address, uint16_t port, const std::string &message)
        {
            return write(
                ip_address,
                port,
                message.c_str(),
                message.length());
        };

    } // namespace Datagram

} // namespace Network
