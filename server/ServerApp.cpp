#include <iostream>
#include <sstream>
#include <shared/shared.h>
#include <chrono>
#include "ServerApp.h"
#include <vector>

Socket newClientSocket;

void ServerApp::Run() {
    mRunning = true;

    mServerSocket = Socket{AF_INET, SOCK_STREAM, IPPROTO_TCP};
  
    // Output hostname
    char hostname[HOST_BUFFER_SIZE] = {0};
    gethostname(hostname, HOST_BUFFER_SIZE - 1);
    std::cout << "Hostname: " << hostname << std::endl;

    // Set up socket server information
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;  
    addr.sin_port = htons(mPortNumber); 

    mServerSocket.Bind(reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
    mServerSocket.Listen(SOMAXCONN);

    std::cout << "Listening on port " << mPortNumber << std::endl;

    AcceptClients();
}

void ServerApp::AcceptClients() {

    sockaddr_in clientSockAddr;
    socklen_t clientSockAddrSize = sizeof(clientSockAddr);

    while (mRunning) {     
        auto clientOpt = mServerSocket.Accept(reinterpret_cast<sockaddr*>(&clientSockAddr), &clientSockAddrSize);
        if (!clientOpt) {continue;}

        Socket clientSocket = std::move(clientOpt.value());

        auto resOpt = clientSocket.Recv();
        if (!resOpt) {continue;}

        std::string validationMessage = resOpt.value();
        size_t delimPos = validationMessage.find(DELIM);
        if (delimPos == std::string::npos) {
            continue;
        }

        std::string appIdentifier = validationMessage.substr(0,delimPos);
        std::string username = validationMessage.substr(delimPos+1);
        
        std::cout << "VALIDATE: " << appIdentifier << " USERNAME: " << username;

        if (appIdentifier == APP_IDENTIFIER) {
            clientSocket.Send(SERVER_CONNECTION_ACCEPTED);
        }
        else {
            clientSocket.Send(SERVER_CONNECTION_DECLINED);
        }
    }
    
}

