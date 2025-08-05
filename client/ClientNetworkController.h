#pragma once 

#include <shared/socket/Socket.h>

class ClientNetworkController {

    Socket mServerSocket;

    bool ValidateServer(const std::string& username);    
public:
    bool ConnectToServer(const std::string& username, const std::string& host, const std::string& port);
};