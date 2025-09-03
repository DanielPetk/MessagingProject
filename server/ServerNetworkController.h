#pragma once

#include <atomic>
#include <optional>
#include <memory>

#include <shared/socket/Socket.h>

class MainInterface;

class ServerNetworkController {

    std::atomic<bool> mRunning = false;
    std::atomic<int> mPort = 54321;
    Socket mServerSocket;
    std::shared_ptr<MainInterface> mInterface = nullptr;

public:
    void SetPort(int port) { mPort = port; } 
    int GetPort() { return mPort; }
    bool GetRunning() { return mRunning; }
    void AddInterface(std::shared_ptr<MainInterface> mainInterface) {mInterface = mainInterface;}
    std::optional<std::string> StartListening();

};