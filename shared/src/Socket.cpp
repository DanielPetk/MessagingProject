#include <stdexcept> 
#include "shared/socket/Socket.h"

Socket::Socket(int af, int type, int protocol) {
    mSocket = ::socket(af, type, protocol);
    if (mSocket == INVALID_SOCKET){
        throw std::runtime_error("Socket failed to create.");
    }
}

Socket::Socket(Socket&& other) { 
    mSocket = other.mSocket;
    other.mSocket = INVALID_SOCKET;
}

Socket& Socket::operator=(Socket&& other) {
    if (this != &other) {
        Close();
        mSocket = other.mSocket;
        other.mSocket = INVALID_SOCKET;
    }
    return *this;
}   

Socket::~Socket() {
    Close();
}

void Socket::Close() {
    if (IsValid()) {
        closesocket(mSocket);
    }
    mSocket = INVALID_SOCKET;
}