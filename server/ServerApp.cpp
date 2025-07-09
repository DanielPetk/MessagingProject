#include <iostream>
#include <sstream>
#include <shared/shared.h>
#include <shared/socket/Socket.h>
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
    std::string messageBuffer(MESSAGE_BUFFER_SIZE, '\0');

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

        // Look for cient app
        int bytesRecieved = recv(clientSocket, messageBuffer.data(), MESSAGE_BUFFER_SIZE, 0);
        if (bytesRecieved <= 0){
            closesocket(clientSocket);
            std::cerr << "Client timed out.";
            continue;
        }

        std::string message = messageBuffer.substr(0, bytesRecieved);
        
        size_t delimPos = message.find(':');
        if (delimPos == std::string::npos) {
            closesocket(clientSocket);
            std::cerr << "Invalid messgae, no delim.";
            continue;
        }

        std::string appIdentifier = message.substr(0,delimPos);
        std::string username = message.substr(delimPos+1);

        if (appIdentifier == APP_IDENTIFIER) {
            std::string response = SERVER_CONNECTION_ACCEPTED;
            send(clientSocket, response.data(), response.length(), 0);
            std::cout << username << " connected!";

            mClientsMutex.lock();
            mClients[clientSocket] = std::thread([&]{
                HandleClient(clientSocket);
            });
            mClientsMutex.unlock();
        } else {
            std::string response = SERVER_CONNECTION_DECLINED;
            send(clientSocket, response.data(), response.length(), 0);
            closesocket(clientSocket);
        }

    }
}

void ServerApp::HandleClient(SOCKET clientSocket) {
    std::cout << "handle";
    // std::string messageBuffer(MESSAGE_BUFFER_SIZE, '\0');
    // std::stringstream messageStream;

    // // Look for signature before we actually initialize chat
    // int bytesRecieved = recv(clientSocket, messageBuffer.data(), MESSAGE_BUFFER_SIZE, 0);
    // if (bytesRecieved <= 0){
    //     return;
    // }

    // std::cout << messageBuffer;


;}