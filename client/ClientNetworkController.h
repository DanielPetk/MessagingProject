#pragma once 

#include <future>
#include <memory>
#include <shared/socket/Socket.h>

class MainInterface;

struct Message {
    Message(const std::string& username, const std::string& message) : mUsername{username}, mMessage{message} {}
    Message(const std::string& username, const std::string& message, bool sentByThisClient) : mUsername{username}, mMessage{message}, mSentByThisClient{sentByThisClient} {}
    std::string mUsername;
    std::string mMessage;
    bool mSentByThisClient = false;
};

class ClientNetworkController {

    Socket mServerSocket;
    std::shared_ptr<MainInterface> mInterface = nullptr;
    
    std::future<void> mConnectingFuture;
    bool ValidateServer(const std::string& username);    
    
public:
    
    ~ClientNetworkController();
    void ConnectToServer(const std::string& username, const std::string& host, const std::string& port);
    void AddInterface(std::shared_ptr<MainInterface>  mainInterface);
};