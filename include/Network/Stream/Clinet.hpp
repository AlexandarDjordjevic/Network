#ifndef THREAD_POOL_H_INC
#define THREAD_POOL_H_INC

#include <memory>

namespace Network
{
    namespace Stream{
    class Client
    {
    public:
        Client();
        ~Client();
        Client(const Client &) = delete;
        Client &operator=(const Client &) = delete;
        Client(Client &&) = delete;
        Client &operator=(Client &&) = delete;

    private:
        
    };        
    } // namespace Stream
} // namespace Network

#endif //THREAD_POOL_H_INC