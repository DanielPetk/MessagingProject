#pragma once 

#include <atomic>
#include <shared/socket/Socket.h>

class ClientNetworkController {

    Socket mServerSocket;
    std::atomic<bool> mConnecting = false;

    bool ValidateServer(const std::string& username);    
public:
    bool ConnectToServer(const std::string& username, const std::string& host, const std::string& port);
};