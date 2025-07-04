#include <iostream>
#include "ServerApp.h"

void ServerApp::Run() {
    mRunning = true;

    // Create a socket
    mServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (mServerSocket == INVALID_SOCKET){
        std::cerr << "Failed to initialize socket.";
        return;
    }

    // Output hostname
    char hostname[HOST_BUFFER_SIZE] = {0};
    gethostname(hostname, HOST_BUFFER_SIZE - 1);
    std::cout << "Hostname: " << hostname << std::endl;

    // Set up socket server information
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY; 
    addr.sin_port = htons(mPortNumber); // Example port

    // Bind the socket to a port
    if (bind(mServerSocket, (sockaddr*) &addr, sizeof(addr)) == SOCKET_ERROR) {
        closesocket(mServerSocket);
        std::cerr << "Failed to bind to port " << mPortNumber << std::endl;
        return;
    }

    if (listen(mServerSocket, SOMAXCONN) == SOCKET_ERROR) {
        closesocket(mServerSocket);
        std::cerr << "Failed to listen on port " << mPortNumber << std::endl;
        return;
    }

    std::cout << "Listening on port " << mPortNumber << std::endl;

    AcceptClients();
}

void ServerApp::AcceptClients() {
    while (mRunning)
    {
        sockaddr_in clientSockAddr;
        socklen_t clientSockAddrSize = sizeof(clientSockAddr);
        
        SOCKET clientSocket = accept(mServerSocket, (sockaddr*) &clientSockAddr, &clientSockAddrSize);
        if (clientSocket == INVALID_SOCKET) {
            closesocket(mServerSocket);
            std::cerr << "Failed to accept incoming connection." << std::endl;
            return;
        }

        std::cout << "connected";
    }
    
}
