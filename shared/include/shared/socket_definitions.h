#pragma once

#include <iostream>
#include <cstdlib>

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
    #include <cerrno>

    using socket_t = int;
    constexpr socket_t INVALID_SOCKET_FD = -1;
    constexpr int SOCKET_ERROR_CODE = -1;
    constexpr int SHUTDOWN_BOTH = SHUT_RDWR;
#endif

inline void netstart() {
#ifdef _WIN32
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (result != 0){
        std::cerr << "Failed to init wsaData";
        std::exit(1);
    }
#endif
}

inline void netshutdown() {
#ifdef _WIN32
    WSACleanup();
#endif
}

inline int socket_close(socket_t socket) {
#ifdef _WIN32
    return closesocket(socket);
#else
    return close(socket);    
#endif
}

inline int getlasterror() {
#ifdef _WIN32
    return WSAGetLastError();
#else
    return errno;    
#endif
}