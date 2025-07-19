#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread> 
#include <mutex>
#include <unordered_map>
#include <string>
#include <shared/socket/Socket.h>

const int HOST_BUFFER_SIZE = 1024;

class ServerApp {
    Socket mServerSocket;
    int mPortNumber = 54321;
    bool mRunning = false;

    void AcceptClients();
    // void HandleClient(SOCKET);
public:
    void Run();
};