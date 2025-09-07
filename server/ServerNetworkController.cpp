#include "ServerNetworkController.h"
#include <thread>
#include <iostream>
constexpr int HOST_BUFFER_SIZE = 1024;

std::optional<std::string> ServerNetworkController::StartListening() {
    if (mRunning) {
        return std::nullopt;
    }
    mRunning = true;
    mServerSocket = Socket{AF_INET, SOCK_STREAM, IPPROTO_TCP};
  
    // get hostname
    std::string hostname(HOST_BUFFER_SIZE, '\0');
    gethostname(hostname.data(), HOST_BUFFER_SIZE - 1);

    // Set up socket server information
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;  
    addr.sin_port = htons(mPort); 

    if (!mServerSocket.Bind(reinterpret_cast<sockaddr*>(&addr), sizeof(addr))) {
        return std::nullopt;
    }

    if (!mServerSocket.Listen(SOMAXCONN)) {
        return std::nullopt;
    }

    
    sockaddr_in clientSockAddr;
    socklen_t clientSockAddrSize = sizeof(clientSockAddr);

    AcceptClients();
    return hostname;
}

void ServerNetworkController::AcceptClients() {
    if (!mInterface) { return; }

    mAcceptThread = std::jthread{[&] {
        
    }};
}