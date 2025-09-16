#pragma once

#include <atomic>
#include <deque>
#include <optional>
#include <memory>
#include <thread>

#include <shared/socket/Socket.h>

class MainInterface;

class Client {

public:

    Client(Socket& socket, const std::string& username);
    ~Client() { CloseConnection(); }
    void CloseConnection();

private:

    const std::string mUsername;
    std::atomic<bool> mRunning = true;
    std::jthread mClientThread;
    Socket mClientSocket;

};

class ServerNetworkController {

public:

    ~ServerNetworkController() { StopServer(); }

    void AddInterface(std::shared_ptr<MainInterface> mainInterface) {mInterface = mainInterface;}
    
    std::optional<std::string> StartListening();
    void StopServer();

    void SetPort(int port) { mPort = port; } 
    int GetPort() { return mPort; }
    bool GetRunning() { return mRunning; }

private:

    std::atomic<bool> mRunning = false;
    std::atomic<bool> mLoopAccept = false;
    std::atomic<int> mPort = 54321;
    std::mutex mClientMutex;
    std::mutex mCleanupMutex;

    Socket mServerSocket;
    std::shared_ptr<MainInterface> mInterface = nullptr;
    std::deque<Client> mClients;


    std::jthread mAcceptThread;

    void AcceptClients();

};