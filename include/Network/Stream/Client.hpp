#pragma once

#include <memory>
#include <Network/Socket.hpp>

namespace Network
{
    namespace Stream
    {
        class Client
        {
        public:
            Client();
            ~Client();
            Client(const Client &) = delete;
            Client &operator=(const Client &) = delete;
            Client(Client &&) = delete;
            Client &operator=(Client &&) = delete;

            /**
             * @brief This function is used to connect client to the remote network node
             * 
             * @param ip_address Remote network node ip address
             * @param port Remote network node port
             * @return true Operation was susccessful
             * @return false Operation was unsucessful
             */
            bool connect(std::string ip_address, uint16_t port);

            /**
             * @brief This function is used to disconnect client from the remote network node
             * 
             */
            void disconnect();

            /**
             * @brief This is the function for sendig messages to the remote network node
             * 
             * @param message Message, reperesented as string
             * @return true Operation was susccessful
             * @return false Operation was unsusccessful
             */
            bool write(const std::string &message);

            /**
             * @brief This is the function for sendig messages to the remote network node
             * 
             * @param message Message, represented as byte array
             * @param length Length of the message
             * @return true Operation was susccessful
             * @return false Operation was unsusccessful
             */
            bool write(const uint8_t *message, size_t length);

            /**
             * @brief This function is used to read incoming data
             * 
             * @param message_buffer Buffer to store incoming data
             * @param buffer_size Buffer size
             * @return size_t Number of received bytes
             */
            size_t read(uint8_t *message_buffer, size_t buffer_size);

            /**
             * @brief This function is used to set socket blocking mode
             * 
             * @param blocking True - set blocking mode, False - set non-blocking mode
             * @return true Operation was susccessful
             * @return false Operation was unsusccessful
             */
            bool setBlockingMode(bool blocking);

        private:
            struct impl;
            std::unique_ptr<impl> pimpl;
        };
    } // namespace Stream
} // namespace Network
