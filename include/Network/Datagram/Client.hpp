#pragma once

#include <memory>

namespace Network
{
    namespace Datagram
    {
        class Client
        {
        public:
            Client();
            ~Client();
            Client(const Client &) = delete;
            Client &operator=(const Client &) = delete;
            Client(const Client &&) = delete;
            Client &operator=(const Client &&) = delete;

            /**
             * @brief This is the function for init datagram client
             * 
             * @return true Operation was successful
             * @return false Operation was unsuccessful
             */
            bool init();

            /**
             * @brief This is the function for writing datagrams represented
             *        as string
             * 
             * @param ip_address Remote node IP address
             * @param port Remote node port
             * @param message Message to write
             * @return true Operation was successful
             * @return false Operation was unsuccessful
             */
            bool write(const std::string &ip_address, uint16_t port, const std::string &message);

            /**
             * @brief This is the function for writing datagrams represented as
             *        byte array
             * 
             * @param ip_address Remote node IP address
             * @param port Remote node port
             * @param message Message to be written
             * @param message_length Message length
             * @return true Operatio was successful
             * @return false Operation was unsuccessful
             */
            bool write(const std::string &ip_address, uint16_t port, const char *message, size_t message_length);

        private:
            struct impl;
            std::unique_ptr<impl> pimpl;
        };
    } // namespace Datagram

} // namespace Network
