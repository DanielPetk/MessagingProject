#pragma once

#include <atomic>
#include <optional>
#include <memory>
#include <thread>

#include <shared/socket/Socket.h>

class MainInterface;

class ServerNetworkController {

public:

    void AddInterface(std::shared_ptr<MainInterface> mainInterface) {mInterface = mainInterface;}
    std::optional<std::string> StartListening();
    void SetPort(int port) { mPort = port; } 
    int GetPort() { return mPort; }
    bool GetRunning() { return mRunning; }

private:

    std::atomic<bool> mRunning = false;
    std::atomic<bool> mLoopAccept = false;

    std::atomic<int> mPort = 54321;
    Socket mServerSocket;
    std::shared_ptr<MainInterface> mInterface = nullptr;

    std::jthread mAcceptThread;

    void AcceptClients();

};