#include "ClientNetworkController.h"
#include <shared/shared.h>

bool ClientNetworkController::ConnectToServer(const std::string& username, const std::string& host, const std::string& port) {
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(std::stoi(port));
    
    hostent* he = gethostbyname(host.c_str());
    if (!he) {
        return false;
    }
    memcpy(&server_addr.sin_addr, he->h_addr, he->h_length);

    // Create socket
    mServerSocket = Socket( AF_INET, SOCK_STREAM, 0 );
    // Connect to server 
    mServerSocket.Connect(reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr));

    return true;
}

