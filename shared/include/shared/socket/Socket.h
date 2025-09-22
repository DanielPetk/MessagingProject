#pragma once

#ifdef _WIN32
    #ifndef NOMINMAX
    #define NOMINMAX
    #endif
    #include <winsock2.h>
    #include <ws2tcpip.h> 
#endif

#include <atomic>
#include <mutex>
#include <optional>
#include <string>
#include <expected>

enum class SocketMode {
    TCP
};

/**
 * NOTE:
 * This class is not meant to be flexible all-encompassingsocket wrapper
 * It is just a nice wrapper for my usecases only
 */
class Socket {

public:

    Socket() {};
    Socket(SocketMode mode);
    
    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;
    
    Socket(Socket&&);
    Socket& operator=(Socket&&);
    
    ~Socket();
    
    std::expected<void, int> SetSendTimeout(int timeout);
    std::expected<void, int> Close();
    std::expected<void, int> Connect(const std::string& host, const std::string& port);
    std::expected<void, int> Bind(const struct sockaddr* addr, socklen_t addrlen);
    std::expected<void, int> Listen(int backlog = SOMAXCONN);
    std::expected<Socket, int> Accept(struct sockaddr* addr, socklen_t* addrlen);
    std::expected<std::string, int> Recv(int flags = 0);
    std::expected<int, int> Send(std::string_view message, int flags = 0);
    std::expected<void, int> Shutdown(int how = SD_BOTH);
    std::expected<std::string, int> GetHostnameHelper();
    
    SOCKET Get() {return mSocket;}
    bool IsValid() {return mSocket != INVALID_SOCKET;}
    
    private:
    
    explicit Socket(SOCKET socket);
    SOCKET mSocket = INVALID_SOCKET;
    std::mutex mCloseMutex;
    int GetLastError();
};