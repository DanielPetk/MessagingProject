#pragma once 

#include <shared/socket/Socket.h>

class ClientNetworkController {

    Socket mServerSocket;

public:

    bool ConnectToServer(const std::string& username, const std::string& host, const std::string& port);
};