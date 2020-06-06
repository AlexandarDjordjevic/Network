#pragma once 

#include <memory>
#include <sys/epoll.h>
#include <Network/Socket.hpp>

namespace Network
{
    class SocketEventManager{
    public:

        typedef void(*dataRecivedDelegate_t)(int fd, uint8_t * data, size_t len);
        typedef void(*disconnectDelegate_t)(int fd, uint32_t reason);

        SocketEventManager();
        ~SocketEventManager();
        SocketEventManager(const SocketEventManager&) = delete;
        SocketEventManager& operator=(const SocketEventManager&) = delete;
        SocketEventManager(const SocketEventManager&&) = delete;
        SocketEventManager& operator=(const SocketEventManager&&) = delete;

        /**
         * @brief This function is used to crate event monitor instance
         * 
         * @return true operation was successful
         * @return false operation was unsuccessful
         */
        bool create();

        /**
         * @brief This function is used to add socket to the list of monitored sockets
         * 
         * @param socket Socket to monitor
         * @return true operation was successful
         * @return false operation was unsuccessful
         */
        bool addSocketForMonitoring(Socket& socket);

        /**
         * @brief This function is used to remove socket from the list of monitored sockets
         * 
         * @param socket_fd sockets file description
         * @return true operation was successful
         * @return false operation was unsuccesful
         */
        bool removeSocketFromMonitoring(int socket_fd);
        /**
         * @brief This is the main event loop
         * 
         */
        void eventLoop();

        /**
         * @brief Set the data received delegate
         * 
         * @param dataRecivedDelegate_t delegate for received data
         */
        void setDataReceivedDelegate(dataRecivedDelegate_t);

        /**
         * @brief Set the disconnect delegate
         * 
         */
        void setDisconnectDelegate(disconnectDelegate_t);
    private:
        struct impl;
        std::unique_ptr<impl> pimpl;
    };
} // namespace Network
