#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread> 
#include <mutex>
#include <unordered_map>
#include <string>

const int HOST_BUFFER_SIZE = 1024;

class ServerApp {
    bool mRunning = false;
    int mPortNumber = 54321;
    SOCKET mServerSocket;
    std::unordered_map<SOCKET, std::thread> mClients;
    std::mutex mClientsMutex;
    void AcceptClients();
    void HandleClient(SOCKET);
public:
    void Run();
};