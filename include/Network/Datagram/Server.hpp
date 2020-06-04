#ifndef THREAD_POOL_H_INC
#define THREAD_POOL_H_INC

#include <memory>

namespace Network
{
    namespace Datagram{
    class Server
    {
    public:
        Server();
        ~Server();
        Server(const Server &) = delete;
        Server &operator=(const Server &) = delete;
        Server(Server &&) = delete;
        Server &operator=(Server &&) = delete;

    private:
        struct impl;
        std::unique_ptr<impl> pimpl;
    };        
    } // namespace Stream
} // namespace Network

#endif //THREAD_POOL_H_INC