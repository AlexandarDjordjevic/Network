# Network

This is the simple network library with following implementations:
1. [Sockets](#1-sockets)
2. [Stream](#2-stream)
   1. [Server](#21-server)
   2. [Client](#22-client)
3. [Datagram](#3-datagram)
   1. [Server](#31-server)
   2. [Clinet](#32-client)
   
## 1. Sockets

---

This is the implemetantion of network communication endpoints (sockets).

#### Example

```cpp
Network::Socket socket;
socket.create(
        Socket::Domain::D_INET,
        Socket::Type::SOCK_STREAM
);
socket.bind(port);
socket.listen();
socket.accept();
```

## 2. Stream

---

### 2.1 Server

---

#### Example

```cpp
Network::SocketEventManager eventManager;
Network::Socket server, client;

void serverTask()
{
    server.create(Network::Socket::Domain::D_INET,
                  Network::Socket::Type::SOCK_STREAM);
    server.bind(1234);
    server.listen();
    while (true)
    {
        if (server.accept(&client))
            eventManager.addSocketForMonitoring(client);
    }
```

### 2.2 Client

---

#### Example

```cpp
Network::Stream::Client client;
std::string ip_address = "127.0.0.1";
uint16_t port = 1234;
uint8_t message_buffer[1024] = {0};

if (client.connect(ip_address, port))
{
   std::cout << "Connected to " << ip_address << ":" << port << std::endl;
}
else
{
   std::cout << "Fail to connect" << std::endl;
}

client.write("Hello From Client\n");

auto readed_length = client.read(message_buffer, sizeof(message_buffer));
std::cout << "Client got " << readed_length << " bytes. Message :" << message_buffer << std::endl;

client.disconnect();
```

## 3. Datagram

---

### 3.1 Server


### 3.2 Client

---

Notes:  

1. Test server for testing connections -> 52.20.16.20 30000  
2. About sockets - https://beej.us/guide/bgnet/html/