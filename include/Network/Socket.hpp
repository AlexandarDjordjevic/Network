#pragma once

#include <stdint.h>
#include <stddef.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

#define DEFAULT_CONNECTION_QUEUE_SIZE 10
namespace Network
{
    class Socket
    {
    public:
        enum struct Domain{
            D_UNSPEC,
            D_LOCAL = 1,
            D_UNIX = 1,
            D_FILE = 1,
            D_INET = 2,	
            D_AX25,	
            D_IPX,	
            D_APPLETALK,
            D_NETROM,
            D_BRIDGE,
            D_ATMPVC,
            D_X25,
            D_INET6,
            D_ROSE,
            D_DECnet,
            D_NETBEUI,
            D_SECURITY,
            D_KEY,
            D_NETLINK,
            D_ROUTE,
            D_PACKET,
            D_ASH,
            D_ECONET,
            D_ATMSVC,
            D_RDS,
            D_SNA,
            D_IRDA,
            D_PPPOX,
            D_WANPIPE,
            D_LLC,
            D_IB,
            D_MPLS,
            D_CAN,
            D_TIPC,
            D_BLUETOOTH,
            D_IUCV,
            D_RXRPC,
            D_ISDN,
            D_PHONET,
            D_IEEE802154,
            D_CAIF,
            D_ALG,
            D_NFC,
            D_VSOCK,
            D_KCM,
            D_QIPCRTR,
            D_SMC,
            D_MAX,
        };
        enum struct Type{
            SOCK_DGRAM,
            SOCK_STREAM
        };

        Socket();
        Socket(int descriptor);
        Socket(Domain, Type);
        ~Socket();
        Socket(const Socket&) = delete;
        Socket& operator=(const Socket&) = delete;
        Socket(Socket&&) = delete;
        Socket& operator=(const Socket&&) = delete;

        /**
         * @brief This is the function for creating the socket
         * 
         * @return true if operation was successful
         * @return false if operation was unsuccessful
         */
        bool create();

        /**
         * @brief This is the function for creating a socket
         * 
         * @param domain Socket domain
         * @param type Socket domain
         * @return true peration was successful
         * @return false operation was unsuccessful
         */
        bool create(Domain domain, Type type);

        /**
         * @brief This is the function for binding the socket
         * 
         * @param port Bind port number
         * @param address Bind address (numeric representation)
         * @return true binding was successful
         * @return false binding was unsuccessful
         */
        bool bind(const std::string& address, uint16_t port);

        /**
         * @brief This is the function for binding the socket to the address 0.0.0.0(INADDR_ANY)
         * 
         * @param port Bind port number
         * @return true binding was successful
         * @return false binding was unsuccessful
         */
        bool bind(uint16_t port);

        /**
         * @brief Set socket into listen state and ready to accept incoming connections.
         *        Maximum queue for incoming connection is default(10)
         * @return true operation was successful
         * @return false operation was unsuccessful
         */
        bool listen();

        /**
         * @brief Set socket into listen state and ready to accept incoming connections.
         *
         * @param backlog maximum length to which the queue of pending connections may grow.
         * @return true operation was successful
         * @return false operation was unsuccessful
         */
        bool listen(size_t backlog);

        /**
         * @brief Set the non blocking mode for the socket
         * 
         * @return true operation was successful
         * @return false operation was unsuccessful
         */
        bool setNonBlockingMode();

        /**
         * @brief This is the function for acceptiong incoming connections
         * 
         * @return int incoming connection socket descriptor
         */
        int accept();

        /**
         * @brief This is the function for acceptiong incoming connections
         * 
         * @param client object to capture socket data
         * @return true opearatio was successful
         * @return false operation was unsuccessful
         */
        bool accept(Socket* client);

        /**
         * @brief This is the function for writing data(binary) on socket
         * 
         * @param data data to be written
         * @param length lenght of the data
         * @return true writing data was successful
         * @return false writing data was unsuccessful 
         */
        bool write(const uint8_t* data, size_t length);

         /**
         * @brief This is the function for writing string on socket
         * 
         * @param data string to be written
         * @return true writing data was successful
         * @return false writing data was unsuccessful 
         */       
        bool write(const std::string& data);

        /**
         * @brief This is the function for reading data from socket
         * 
         * @param buffer Data storing buffer
         * @param length Data buffer size
         * @return size_t Number of stored data into data buffer
         */
        size_t read(uint8_t* buffer, size_t length);

        /**
         * @brief This function return socket file descriptor
         * 
         * @return int socket file descriptor
         */
        int getDescriptor() const;

        /**
         * @brief This function return socket ip represented as string
         * 
         * @return std::string address represented as string
         */
        std::string getIp();

        /**
         * @brief This function return socket ip address as numeric representation
         * 
         * @return uint32_t address value
         */
        uint32_t getIpNumeric();

        /**
         * @brief This function return socket port number
         * 
         * @return uint16_t port number
         */
        uint16_t getPort();

        /**
         * @brief This function will cause all or part of a full-duplex connection on the
         *        socket associated with the file descriptor socket to be shut down.
         * 
         */
        void shutdown();

        /**
         * @brief This function will dealocate socket file
         * 
         */
        void close();

    private:
        int fileDescriptor;
        uint32_t address;
        in_port_t port;
        Socket::Domain domain;
        Socket::Type type;
    };
    

    
} // namespace Network
