#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread> 
#include <unordered_map>

const int HOST_BUFFER_SIZE = 1024;

class ServerApp {
    bool mRunning = false;
    int mPortNumber = 54321;
    SOCKET mServerSocket;
    std::unordered_map<SOCKET, std::thread> mClients;
    void AcceptClients();
public:
    void Run();
};