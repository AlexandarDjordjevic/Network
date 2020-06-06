#ifndef THREAD_POOL_H_INC
#define THREAD_POOL_H_INC

#include <stdint.h>
#include <memory>
#include <vector>
#include <Network/Socket.hpp>

namespace Network
{
    namespace Stream{
        class Server
        {
            enum struct Error{
                NO_ERROR,
                CREATE_SOCKET_ERROR,
                BIND_SOCKET_ERROR,
                LISTEN_SOCKET_ERROR,
                ACCEPT_SOCKET_ERROR

            };
        public:
            typedef void(*readDataDelegate_t)(char*, size_t);

            Server();
            ~Server();
            Server(const Server &) = delete;
            Server &operator=(const Server &) = delete;
            Server(Server &&) = delete;
            Server &operator=(Server &&) = delete;

            bool listen(uint16_t port);
            bool listen(std::string address, uint16_t port);
            void accept();
            void setReceivedDataDelegate(readDataDelegate_t);
            std::string getLastError();
        private:
            Socket serverSocket;
            std::vector<std::shared_ptr<Socket>> clients;
            readDataDelegate_t receiveHandler;
            Error error;
            bool run;

        };        
    } // namespace Stream
} // namespace Network

#endif //THREAD_POOL_H_INC