#include <iostream>
#include <stdexcept> 
#include "shared/socket/Socket.h"
#include "shared/shared.h"

Socket::Socket(SOCKET socket) {
    if (socket == INVALID_SOCKET) {
        throw std::runtime_error("Socket failed to create.");      
    }

    int optVal = 0;
    int optLen = sizeof(optVal);
    if (getsockopt(socket, SOL_SOCKET, SO_TYPE, reinterpret_cast<char*>(&optVal), &optLen) == SOCKET_ERROR) {
        throw std::runtime_error("Socket failed to create.");
    }

    mSocket = socket;
}

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

void Socket::UpdateValidity() {
    int err = WSAGetLastError();
    if (err == WSAEBADF || err == WSAENOTSOCK){
        mSocket = INVALID_SOCKET;
    }
}

void Socket::Bind(const struct sockaddr* addr, socklen_t addrlen) {
    if (bind(mSocket, addr, addrlen) == SOCKET_ERROR) {
        UpdateValidity();
        throw std::runtime_error("Socket failed to bind.");
    }
}

void Socket::Listen(int backlog) {
    if (listen(mSocket, backlog) == SOCKET_ERROR) {
        UpdateValidity();
        throw std::runtime_error("Socket failed to listen.");
    }
}

std::optional<Socket> Socket::Accept(struct sockaddr* addr, socklen_t* addrlen) {
    SOCKET otherSocket = accept(mSocket, addr, addrlen);
    if (otherSocket != INVALID_SOCKET){
        return Socket{otherSocket};
    }

    int err = WSAGetLastError();
    if (err == WSAEWOULDBLOCK) {
        return std::nullopt; // Non-blocking and no connection available so no fatal error
    } else {
        UpdateValidity();
        throw std::runtime_error("Socket failed to accept.");
    }
}

std::optional<std::string> Socket::Recv(int flags) {
    char buffer[MESSAGE_BUFFER_SIZE] = "\0";
    int bytesRecieved = recv(mSocket, buffer, MESSAGE_BUFFER_SIZE, flags);

    if (bytesRecieved >= 0) {
        return std::string(buffer, bytesRecieved);
    }

    int err = WSAGetLastError();
    if (err == WSAEWOULDBLOCK) {
        return std::nullopt; // Non-blocking and no connection available so no fatal error
    } else {
        UpdateValidity();
        throw std::runtime_error("Socket failed to recv.");
    }
}

std::optional<int> Socket::Send(std::string_view message, int flags) {
    int bytesSent = send(mSocket, message.data(), message.length(), flags);

    if (bytesSent >= 0) {
        return bytesSent;
    }

    int err = WSAGetLastError();
    if (err == WSAEWOULDBLOCK) {
        return std::nullopt; // Non-blocking and no connection available so no fatal error
    } else {
        UpdateValidity();
        throw std::runtime_error("Socket failed to send.");
    }
}