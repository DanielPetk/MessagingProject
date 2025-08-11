#pragma once 

#include <atomic>
#include <future>
#include <shared/socket/Socket.h>

class ClientNetworkController {

    Socket mServerSocket;
    std::future<void> mConnectingFuture;

    bool ValidateServer(const std::string& username);    

public:

    // Network -> UI on completion of connection
    std::function<void(bool)> mConnectingDone;    

    void ConnectToServer(std::string username, std::string host, std::string port);
};