#include <Network/SocketEventManager.hpp>
#include <unordered_map>
#include <mutex>
#include <thread>
namespace Network
{
    static const int INVALID_FD = -1;
    struct SocketEventManager::impl{
        bool run;
        std::mutex runMutex;
        int monitor_fd = INVALID_FD;
        dataRecivedDelegate_t dataReceivedDelegate;
        disconnectDelegate_t disconnectDelegate;
        struct epoll_event events[10]; //TODO refactor this!
        size_t max_events = 10; //TODO refactor this!
    };

    SocketEventManager::SocketEventManager()
        : pimpl(new SocketEventManager::impl())
    {
        pimpl->run = true;
    }
    SocketEventManager::~SocketEventManager() = default;

    bool SocketEventManager::create(){
        pimpl->monitor_fd = epoll_create1(0);
        if(pimpl->monitor_fd == INVALID_FD) return false;
        return true;
    }

    void SocketEventManager::setDataReceivedDelegate(dataRecivedDelegate_t delegate){
        pimpl->dataReceivedDelegate = delegate;
    }

    void SocketEventManager::setDisconnectDelegate(disconnectDelegate_t delegate){
        pimpl->disconnectDelegate = delegate;
    }

    bool SocketEventManager::addSocketForMonitoring(Socket& socket){
        struct epoll_event ev;
        ev.events = EPOLLIN | EPOLLRDHUP | EPOLLHUP | EPOLLOUT | EPOLLERR; 
        ev.data.fd = socket.getDescriptor();
        if (epoll_ctl(pimpl->monitor_fd, EPOLL_CTL_ADD, socket.getDescriptor(), &ev) == -1)
        {
            return false;
        }
        return true;
    }

    bool SocketEventManager::removeSocketFromMonitoring(int socket_fd){
        if (epoll_ctl(pimpl->monitor_fd, EPOLL_CTL_DEL, socket_fd, nullptr) == -1) return false;
        return true;
    }

    void SocketEventManager::stop(){
        std::lock_guard<std::mutex> lock(pimpl->runMutex);
        pimpl->run = false;
    }

    void SocketEventManager::eventLoop(){
        while(pimpl->run){
            auto eventNumber = epoll_wait(pimpl->monitor_fd, pimpl->events, pimpl->max_events, -1); 
            if (eventNumber == -1){
                //Error epoll_wait ... What to do with this?
            } else {
                for (size_t i = 0; i < eventNumber; i++){
                    if ((pimpl->events[i].events & EPOLLERR) == EPOLLERR){ 
                        if (pimpl->disconnectDelegate) 
                            pimpl->disconnectDelegate(pimpl->events[i].data.fd, EPOLLERR);
                            removeSocketFromMonitoring(pimpl->events[i].data.fd);
                    } else 
                    if ((pimpl->events[i].events & EPOLLRDHUP) == EPOLLRDHUP){ 
                        if (pimpl->disconnectDelegate) 
                            pimpl->disconnectDelegate(pimpl->events[i].data.fd, EPOLLRDHUP);
                            removeSocketFromMonitoring(pimpl->events[i].data.fd);
                    } else 
                    if ((pimpl->events[i].events & EPOLLHUP) == EPOLLHUP){ 
                        if (pimpl->disconnectDelegate) 
                            pimpl->disconnectDelegate(pimpl->events[i].data.fd, EPOLLHUP);
                            removeSocketFromMonitoring(pimpl->events[i].data.fd);
                    } else 
                    if ((pimpl->events[i].events & EPOLLIN) == EPOLLIN){
                        uint8_t buffer[2048];
                        size_t lenght;
                        lenght = read(pimpl->events[i].data.fd, buffer, sizeof(buffer));
                        if (lenght > 0){
                            if (pimpl->dataReceivedDelegate){
                                pimpl->dataReceivedDelegate(
                                    pimpl->events[i].data.fd, 
                                    buffer, 
                                    lenght);
                            }
                        }
                    } else 
                    if ((pimpl->events[i].events & EPOLLOUT) == EPOLLOUT){ 

                    } else {
                        //Invalid event...
                    }     
                }
            }

        }
    }

} // namespace Network
