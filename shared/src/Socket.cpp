#include <iostream>
#include <stdexcept> 
#include <cstring>
#include "shared/socket/Socket.h"
#include "shared/shared.h"

constexpr int HOST_BUFFER_SIZE = 1024;

Socket::Socket(socket_t socket) {
    if (socket == INVALID_SOCKET_FD) {
        throw std::runtime_error("Socket failed to create.");      
    }

    int optVal = 0;
    socklen_t optLen = sizeof(optVal);
    if (getsockopt(socket, SOL_SOCKET, SO_TYPE, reinterpret_cast<char*>(&optVal), &optLen) == SOCKET_ERROR_CODE) {
        throw std::runtime_error("Socket failed to create.");
    }

    mSocket = socket;
}

// Only really have one option so the argument is a bit useless
// I just use it to differentiate from default constructor
Socket::Socket(SocketMode mode) {
    mSocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (mSocket == INVALID_SOCKET_FD){
        throw std::runtime_error("Socket failed to create.");
    }
}

Socket::Socket(Socket&& other) { 
    mSocket = other.mSocket;
    other.mSocket = INVALID_SOCKET_FD;
}

Socket& Socket::operator=(Socket&& other) {
    if (this != &other) {
        Close();
        mSocket = other.mSocket;
        other.mSocket = INVALID_SOCKET_FD;
    }
    return *this;
}   

Socket::~Socket() {
    Close();
}

std::expected<void, int> Socket::SetSendTimeout(int timeout) {
#ifdef _WIN32
    if (setsockopt(mSocket, SOL_SOCKET, SO_SNDTIMEO, reinterpret_cast<const char*>(&timeout), sizeof(timeout)) == SOCKET_ERROR_CODE) {
        return std::unexpected{getlasterror()};
    }
#else
    struct timeval tv;
    tv.tv_sec = timeoutMs / 1000;
    tv.tv_usec = (timeoutMs % 1000) * 1000;
    if (setsockopt(mSocket, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv)) == SOCKET_ERROR_CODE) {
        return std::unexpected{getlasterror()};
    }
#endif
    return {};
}

std::expected<void, int> Socket::Close() {
    if (IsValid() && socket_close(mSocket) == SOCKET_ERROR_CODE) {
        return std::unexpected{getlasterror()};
    } 
    {
        std::lock_guard<std::mutex> m{mCloseMutex};
        mSocket = INVALID_SOCKET_FD;
    }
    return {};
}

std::expected<void, int> Socket::Connect(const std::string& host, int port) {
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
        
    hostent* he = gethostbyname(host.c_str());
    if (!he) {
        return std::unexpected{getlasterror()};
    }
    std::memcpy(&server_addr.sin_addr, he->h_addr, he->h_length);

    if (connect(mSocket, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr)) == SOCKET_ERROR_CODE){
        return std::unexpected{getlasterror()};
    }    
    return {};
}

std::expected<void, int> Socket::Bind(int port) {
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;  
    addr.sin_port = htons(port); 

    if (bind(mSocket, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == SOCKET_ERROR_CODE) {
        return std::unexpected{getlasterror()};
    }

    return {};
}

std::expected<void, int> Socket::Listen(int backlog) {
    if (listen(mSocket, backlog) == SOCKET_ERROR_CODE) {
        return std::unexpected{getlasterror()};
    }
    return {};
}

std::expected<Socket, int> Socket::Accept() {
    
    sockaddr_in clientSockAddr;
    socklen_t clientSockAddrSize = sizeof(clientSockAddr);

    socket_t otherSocket = accept(mSocket, reinterpret_cast<sockaddr*>(&clientSockAddr), &clientSockAddrSize);
    if (otherSocket != INVALID_SOCKET_FD){
        return Socket{otherSocket};
    }
    return std::unexpected{getlasterror()};
}

std::expected<std::string, int> Socket::Recv(int flags) {
    char buffer[MESSAGE_BUFFER_SIZE] = "\0";
    int bytesRecieved = recv(mSocket, buffer, MESSAGE_BUFFER_SIZE, flags);

    if (bytesRecieved >= 0) {
        return std::string(buffer, bytesRecieved);
    }
    return std::unexpected{getlasterror()};
}

std::expected<int, int> Socket::Send(std::string_view message, int flags) {
    int bytesSent = send(mSocket, message.data(), message.length(), flags);
    if (bytesSent >= 0) {
        return bytesSent;
    }
    return std::unexpected{getlasterror()};
}

std::expected<void, int> Socket::Shutdown(int how) {
    if (shutdown(mSocket, how) == SOCKET_ERROR_CODE){
        return std::unexpected{getlasterror()};
    }    
    return {};
}

std::expected<std::string, int> Socket::GetHostnameHelper() {
    std::string hostname(HOST_BUFFER_SIZE, '\0');
    if (gethostname(hostname.data(), HOST_BUFFER_SIZE - 1) == SOCKET_ERROR_CODE) {
        return std::unexpected{getlasterror()};
    }
    return hostname;
}