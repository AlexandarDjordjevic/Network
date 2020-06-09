#pragma once

#include <memory>

namespace Network
{
    namespace Datagram
    {
        class Server
        {
        public:
            typedef void (*dataRecieveDelegate_t)(uint8_t *buffer, size_t length);

            Server();
            ~Server();

            /**
             * @brief This is the function used to start receive datagrams 
             * 
             * @param port Port for listening
             */
            bool startReceive(uint16_t port);

            /**
             * @brief This is the server main loop.
             * 
             */
            void run();

            /**
             * @brief This function is used to set data receive delegate
             * 
             */
            void setDataReceiveDelegate(dataRecieveDelegate_t);

        private:
            struct impl;
            std::unique_ptr<impl> pimpl;
        };
    } // namespace Datagram

} // namespace Network
