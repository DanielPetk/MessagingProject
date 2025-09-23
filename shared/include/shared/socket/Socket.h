#pragma once

// Here we define all of the OS differences
#ifdef _WIN32
    #ifndef NOMINMAX
    #define NOMINMAX
    #endif
    #include <winsock2.h>
    #include <ws2tcpip.h> 

    using socket_t = SOCKET;
    constexpr socket_t INVALID_SOCKET_FD = INVALID_SOCKET;
    constexpr int SOCKET_ERROR_CODE = SOCKET_ERROR;
    constexpr int SHUTDOWN_BOTH = SD_BOTH;
#else
    #include <netdb.h>
    #include <unistd.h>
    #include <arpa/inet.h>

    using socket_t = int;
    constexpr socket_t INVALID_SOCKET_FD = -1;
    constexpr int SOCKET_ERROR_CODE = -1;
    constexpr int SHUTDOWN_BOTH = SHUT_RDWR;
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
    std::expected<void, int> Connect(const std::string& host, int port);
    std::expected<void, int> Bind(int port);
    std::expected<void, int> Listen(int backlog = SOMAXCONN);
    std::expected<Socket, int> Accept();
    std::expected<std::string, int> Recv(int flags = 0);
    std::expected<int, int> Send(std::string_view message, int flags = 0);
    std::expected<void, int> Shutdown(int how = SHUTDOWN_BOTH);
    std::expected<std::string, int> GetHostnameHelper();
    
    bool IsValid() {return mSocket != INVALID_SOCKET_FD;}
    
    private:
    
    explicit Socket(socket_t socket);
    socket_t mSocket = INVALID_SOCKET_FD;
    std::mutex mCloseMutex;
    int GetLastError();
};