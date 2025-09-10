#pragma once

#include <atomic>
#include <thread>
#include <memory>
#include <mutex>

#include <shared/socket/Socket.h>

class MainInterface;

enum class MessageType {
    Received,
    Sent,
    Error
};

struct Message {
    Message(const std::string& username, const std::string& message) : mUsername{username}, mMessage{message} {}
    Message(const std::string& username, const std::string& message, MessageType messageType) : mUsername{username}, mMessage{message}, mMessageType{messageType} {}
    std::string mUsername;
    std::string mMessage;
    MessageType mMessageType = MessageType::Received;
};

class ClientNetworkController {

public:

    ~ClientNetworkController();
    void ConnectToServer(const std::string& username, const std::string& host, const std::string& port);
    void AddInterface(std::shared_ptr<MainInterface> mainInterface);
    void StartReceiveMessageLoop();
    bool SendServerMessage(const std::string& message);
    void CloseServerConnection();

    void SetRunning(bool running) { mRunning = running; }
private:

    Socket mServerSocket;
    std::shared_ptr<MainInterface> mInterface = nullptr;
    std::string mUsername;

    std::mutex mCleanupMutex;
    std::atomic<bool> mLoopRecv = false;
    std::atomic<bool> mRunning = false;
    
    std::jthread mConnectingThread;
    std::jthread mReceivingMessageThread;

    bool ValidateServer(const std::string& username);    
    
};
