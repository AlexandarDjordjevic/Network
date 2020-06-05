#include <Network/Socket.hpp>


namespace Network
{
    Socket::Socket(int descriptor){
        fileDescriptor = descriptor;
    }

    Socket::Socket(Domain _domain, Type _type)
        : domain(_domain), type(_type)
    {
        fileDescriptor = -1; //Invalid socket
    }   
    Socket::~Socket(){
        shutdown();
        //TODO Add read before closing socket ... test if it is necessary read!
        close();
    } 

    bool Socket::create(){
        fileDescriptor = socket(int(domain), int(type), 0);
        if (fileDescriptor < 0) return false;
        return true;
    }

    bool Socket::bind(uint16_t port, uint32_t address){
        if (fileDescriptor < 0) return false;

        this->address = address;
        this->port = port;

        struct sockaddr_in serv_addr;
        serv_addr.sin_family = sa_family_t(domain);
        serv_addr.sin_addr.s_addr = htonl(this->address);
        serv_addr.sin_port = htons(this->port); 
 
        auto result = ::bind(fileDescriptor, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 
        if (result < 0) return false;
        return true;
    }   

    bool Socket::bind(uint16_t port){
        return bind(port, INADDR_ANY);
    }   

    bool Socket::listen(){
        if (fileDescriptor < 0) return false;
        auto result = ::listen(fileDescriptor, DEFAULT_CONNECTION_QUEUE_SIZE);
        if(result < 0) return false;
        return true;
    }

    bool Socket::listen(size_t backlog){
        if (fileDescriptor < 0) return false;
        auto result = ::listen(fileDescriptor, backlog);
        if(result < 0) return false;
        return true;
    }

    bool Socket::setNonBlockingMode(){
        if (fileDescriptor == -1) return false;
        int flags = fcntl(fileDescriptor, F_GETFL, 0);
        if (flags == -1) return false;
        flags |= O_NONBLOCK;
        return (fcntl(fileDescriptor, F_SETFL, flags) == 0) ? true : false;
    }

    int Socket::accept(){
        return ::accept(fileDescriptor, NULL, NULL);
    }

    bool Socket::accept(Socket& client){
        struct sockaddr_in cli;
        socklen_t len = 0;
        client.fileDescriptor = ::accept(fileDescriptor, (struct sockaddr*)&cli, &len);
        client.address = ntohl(cli.sin_addr.s_addr);
        client.port= ntohs(cli.sin_port);
    }

    bool Socket::write(const uint8_t* data, size_t length){
        if (fileDescriptor < 0) return false;
        auto result = ::write(fileDescriptor, data, length);
        if (result == -1) return false;
        return true;
    }

    bool Socket::write(const std::string& data){
        return write((const uint8_t*)(data.c_str()), data.length());
    }

    int Socket::getDescriptor() const{
        return fileDescriptor;
    }

    std::string Socket::getIp(){
        char str[INET_ADDRSTRLEN] = {0};
        try{
            in_addr clientAdd;
            clientAdd.s_addr = htonl(address);
            inet_ntop(AF_INET, &(clientAdd), str, INET_ADDRSTRLEN);
            return std::string(str);
        }catch(...){
            return nullptr;
        }
    }

    uint32_t Socket::getIpNumeric(){
        return address;
    }

    uint16_t Socket::getPort(){
        return port;
    }

    void Socket::shutdown(){
        ::shutdown(fileDescriptor, SHUT_RDWR);
    }

    void Socket::close(){
        ::close(fileDescriptor);
    }

} // namespace Network
