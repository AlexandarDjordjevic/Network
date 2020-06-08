#include <Network/Stream/Client.hpp>

namespace Network
{
    namespace Stream
    {

        struct Client::impl
        {
            Socket socket;
        };
        Client::Client()
            : pimpl(new Client::impl)
        {
        }

        Client::~Client()
        {
        }

        bool Client::connect(std::string ip_address, uint16_t port)
        {
            if (false == pimpl->socket.create(
                             Network::Socket::Domain::D_INET,
                             Network::Socket::Type::SOCK_STREAM))
            {
                return false;
            }
            return pimpl->socket.connect(ip_address, port);
        }

        void Client::disconnect()
        {
            pimpl->socket.shutdown();
            pimpl->socket.close();
        }

        bool Client::write(const std::string &message)
        {
            pimpl->socket.write(message);
        }

        bool Client::write(const uint8_t *message, size_t length)
        {
            pimpl->socket.write(message, length);
        }

        size_t Client::read(uint8_t *message_buffer, size_t buffer_size)
        {
            return pimpl->socket.read(message_buffer, buffer_size);
        }

        bool Client::setBlockingMode(bool blocking)
        {
            return pimpl->socket.setBlockingMode(blocking);
        }
    } // namespace Stream

} // namespace Network
