# Network

This is the simple network library with following implementations:
1. [Sockets](#1-sockets)
2. [Stream](#2-stream)
   1. [Server](#21-server)
   2. [Client](#22-client)
3. [Datagram]
   1. [Server]
   2. [Clinet]
   
## 1. Sockets

---

This is the implemetantion of network communication endpoints (sockets).

#### Example

``` 
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

***

### 2.2 Client

***

## Datagram

