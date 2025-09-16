#pragma once

#include <atomic>
#include <mutex>
#include <optional>
#include <string>
#include <expected>
#include <winsock2.h>
#include <ws2tcpip.h>

class Socket {

public:

    Socket() {};
    explicit Socket(SOCKET socket);
    Socket(int af, int type, int protocol);
    
    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;
    
    Socket(Socket&&);
    Socket& operator=(Socket&&);
    
    ~Socket();
    
    std::expected<void, int> SetSendTimeout(int timeout);
    std::expected<void, int> Close();
    std::expected<void, int> Connect(const struct sockaddr* addr, socklen_t addrlen);
    std::expected<void, int> Bind(const struct sockaddr* addr, socklen_t addrlen);
    std::expected<void, int> Listen(int backlog);
    std::expected<Socket, int> Accept(struct sockaddr* addr, socklen_t* addrlen);
    std::expected<std::string, int> Recv(int flags = 0);
    std::expected<int, int> Send(std::string_view message, int flags = 0);
    std::expected<void, int> Shutdown(int how);
    
    SOCKET Get() {return mSocket;}
    bool IsValid() {return mSocket != INVALID_SOCKET;}
    
private:
    
    SOCKET mSocket = INVALID_SOCKET;
    std::mutex mCloseMutex;
    int GetLastError();
};