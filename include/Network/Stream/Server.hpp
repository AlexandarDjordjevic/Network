#pragma once

#include <stdint.h>
#include <memory>
#include <vector>
#include <mutex>
#include <Network/Socket.hpp>
#include <Network/Stream/Client.hpp>
#include <Network/SocketEventManager.hpp>

#include <iostream>
namespace Network
{
    namespace Stream
    {
        class Server
        {
            enum struct Error
            {
                NO_ERROR,
                CREATE_SOCKET_ERROR,
                BIND_SOCKET_ERROR,
                LISTEN_SOCKET_ERROR,
                ACCEPT_SOCKET_ERROR

            };

        public:
            Server();
            ~Server();
            Server(const Server &) = delete;
            Server &operator=(const Server &) = delete;
            Server(Server &&) = delete;
            Server &operator=(Server &&) = delete;

            /**
             * @brief This function is used to setup server socket listener
             * 
             * @param port port for incoming connections
             * @return true operation was successful
             * @return false operation was unsuccessful
             */
            bool listen(uint16_t port);

            /**
             * @brief This function is used to setup server socket listener
             * 
             * @param address address endpoint
             * @param port  port for incoming connections
             * @return true operation was successful
             * @return false operation was unsuccessful
             */
            bool listen(std::string address, uint16_t port);

            /**
             * @brief This function is used to accept incoming connections
             * 
             */
            void accept();
            void enqueClient(const std::shared_ptr<Client> &client);
            void setReceivedDataDelegate(SocketEventManager::dataReceiveDelegate_t);
            void setClientDisconnectedDelegate(SocketEventManager::disconnectDelegate_t);
            bool eventManager();
            /**
             * @brief This function is called when server receive data
             * 
             * @param socket_fd Socket File Descriptor
             * @param data Data received
             * @param len Data length
             */
            void dataReceived(int socket_fd, uint8_t *data, size_t len);

            /**
             * @brief This function is called whenever client disconnects
             * 
             * @param socket_fd Disconnected client socker file descriptor
             * @param reason Disconnect reason
             */
            void clientDisconnected(int socket_fd, uint32_t reason);

            /**
             * @brief This function is used to get last server error description
             * 
             * @return std::string error description
             */
            std::string getLastError();

        private:
            struct impl;
            std::unique_ptr<impl> pimpl;
        };
    } // namespace Stream
} // namespace Network
