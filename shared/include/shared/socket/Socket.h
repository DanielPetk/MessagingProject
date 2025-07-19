#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <optional>

class Socket {
    SOCKET mSocket = INVALID_SOCKET;
    void UpdateValidity();
public:
    Socket() {};
    explicit Socket(SOCKET socket);
    Socket(int af, int type, int protocol);

    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;

    Socket(Socket&&);
    Socket& operator=(Socket&&);

    ~Socket();
   
    void Close();
    void Bind(const struct sockaddr* addr, socklen_t addrlen);
    void Listen(int backlog);
    std::optional<Socket> Accept(struct sockaddr* addr, socklen_t* addrlen);
    std::optional<std::string> Recv(int flags = 0);
    std::optional<int> Send(std::string_view message, int flags = 0);
    SOCKET Get() {return mSocket;}
    bool IsValid() {return mSocket != INVALID_SOCKET;}
};