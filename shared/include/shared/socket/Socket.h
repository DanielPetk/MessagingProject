#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>

class Socket {
    SOCKET mSocket = INVALID_SOCKET;
public:
    Socket(int af, int type, int protocol);

    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;

    Socket(Socket&&);
    Socket& operator=(Socket&&);

    ~Socket();
   
    void Close();
    SOCKET Get() {return mSocket;}
    bool IsValid() {return mSocket != INVALID_SOCKET;}
};