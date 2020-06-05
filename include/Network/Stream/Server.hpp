#ifndef THREAD_POOL_H_INC
#define THREAD_POOL_H_INC

#include <stdint.h>
#include <memory>
// #include <Network/Socket.hpp>

namespace Network
{
    namespace Stream{
        class Server
        {
        public:
            typedef void(*receiveHandler_t)(char*, size_t);

            Server();
            ~Server();
            Server(const Server &) = delete;
            Server &operator=(const Server &) = delete;
            Server(Server &&) = delete;
            Server &operator=(Server &&) = delete;

            void listen(uint16_t port);
            void setReceiveHandler(receiveHandler_t);
        private:
            uint16_t port;
            receiveHandler_t receiveHandler;
            // Socket socket;
        };        
    } // namespace Stream
} // namespace Network

#endif //THREAD_POOL_H_INC