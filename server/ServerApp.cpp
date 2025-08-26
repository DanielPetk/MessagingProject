#include <iostream>
#include <sstream>
#include <shared/shared.h>
#include <shared/protocol/ProtocolHandler.h>
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
        
        ProtocolHandler handler;

        auto clientOpt = mServerSocket.Accept(reinterpret_cast<sockaddr*>(&clientSockAddr), &clientSockAddrSize);
        if (!clientOpt) {continue;}

        Socket clientSocket = std::move(clientOpt.value());

        auto resOpt = clientSocket.Recv();
        if (!resOpt) {continue;}

        std::string validationMessage = resOpt.value();
        std::cout << validationMessage;
        auto parsed = handler.ParseProtocolString(validationMessage);
        std::cout << "\n";
        std::cout << "SIZE" << " " << parsed.size() << std::endl;
        for (const auto& pair : parsed) {
            std::cout << pair.first << " : " << pair.second << "\n";
        }

        if (((parsed.contains(TYPE)) && (parsed[TYPE] == VALIDATE) && (parsed.contains(MESSAGE)) && (parsed[MESSAGE] == APP_IDENTIFIER))) {
            clientSocket.Send(handler.CreateProtocolString({{TYPE, VALIDATE}, {MESSAGE, SERVER_CONNECTION_ACCEPTED}}));
        }
        else {
            clientSocket.Send(handler.CreateProtocolString({{TYPE, VALIDATE}, {MESSAGE, SERVER_CONNECTION_DECLINED}}));
        }
    }
    
}

