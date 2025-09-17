#pragma once

#include <atomic>
#include <optional>
#include <memory>
#include <thread>
#include <vector>

#include <shared/socket/Socket.h>

class MainInterface;
class ServerNetworkController;

class Client {

public:

    Client(Socket& socket, const std::string& username, ServerNetworkController* network);
    ~Client() { CloseConnection(); }
    void CloseConnection();
    void SendMessageToOtherClients(const std::string& message);
    void SendMessageToThisClient(const std::string& message);

private:

    const std::string mUsername;
    Socket mClientSocket;
    ServerNetworkController* mNetwork = nullptr;

    std::atomic<bool> mRunning = true;
    std::jthread mClientThread;

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
    bool GetShuttingDown() { return mShuttingDown; }
    std::vector<std::unique_ptr<Client>>& GetClients() { return mClients; }
    std::mutex& GetClientsMutex() { return mClientMutex; }

private:

    std::atomic<bool> mRunning = false;
    std::atomic<bool> mLoopAccept = false;
    std::atomic<bool> mShuttingDown = false;
    std::atomic<int> mPort = 54321;
    std::mutex mClientMutex;
    std::mutex mCleanupMutex;

    Socket mServerSocket;
    std::shared_ptr<MainInterface> mInterface = nullptr;
    std::vector<std::unique_ptr<Client>> mClients;


    std::jthread mAcceptThread;

    void AcceptClients();

};