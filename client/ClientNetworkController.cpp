#include <chrono>
#include <iostream>

#include "ClientNetworkController.h"
#include "MainInterface.h"
#include <shared/shared.h>
#include <shared/protocol/ProtocolHandler.h>


ClientNetworkController::~ClientNetworkController() {
    CloseServerConnection();
}

void ClientNetworkController::StartReceiveMessageLoop() {
    if (!mInterface || mLoopRecv) { return; }
    mLoopRecv = true;
    ProtocolHandler handler;

    mReceivingMessageThread = std::jthread([=, this] {
        while (mLoopRecv) {
            auto recvRes = mServerSocket.Recv();
            if (!recvRes || recvRes.value() == "") {
                mInterface->OnLoopError();
            }

            mInterface->OnReceivedMessage({"PENIS", "ASS"});
        }
    });
}

void ClientNetworkController::ConnectToServer(const std::string& username, const std::string& host, const std::string& port) {    
    if (!mInterface || mRunning) { return; }
    mRunning = true;

    mConnectingThread = std::jthread([=, this] {    
        struct sockaddr_in server_addr;
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(std::stoi(port));
        
        hostent* he = gethostbyname(host.c_str());
        if (!he) {
            mInterface->OnConnectionError();
            return;
        }
        memcpy(&server_addr.sin_addr, he->h_addr, he->h_length);
        
        // Create socket
        mServerSocket = Socket( AF_INET, SOCK_STREAM, 0 );
        
        if (!mServerSocket.SetSendTimeout(2500)) {
            mServerSocket.Close();
            mInterface->OnConnectionError();
            return;
        }
        
        // Connect to server 
        if (!mServerSocket.Connect(reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr))) {
            mInterface->OnConnectionError();
            return;
        }
        
        if(ValidateServer(username)) {
            mUsername = username;
            mInterface->OnConnectionSuccess();
        }
        else {
            mInterface->OnConnectionError();
        }
    });   
}

void ClientNetworkController::CloseServerConnection() {
    std::lock_guard<std::mutex> m{mCleanupMutex};
    mServerSocket.Close();
    mLoopRecv = false;
    if (mConnectingThread.joinable()) { mConnectingThread.join(); }
    if (mReceivingMessageThread.joinable()) {mReceivingMessageThread.join(); }
    mRunning = false;
}

bool ClientNetworkController::SendServerMessage(const std::string& message) {
    auto sendRes = mServerSocket.Send(message);
    return !(!sendRes || sendRes.value() == 0);
}

bool ClientNetworkController::ValidateServer(const std::string& username) {
    ProtocolHandler handler;        
    std::string validationMessage = handler.CreateProtocolString({{TYPE, VALIDATE}, {MESSAGE, APP_IDENTIFIER}, {USERNAME, username}});
    auto sendRes = mServerSocket.Send(validationMessage, 0);
    
    // If error or if the other side of the connection is closed
    if (!sendRes || sendRes.value() == 0) {
        mServerSocket.Close();
        return false;
    }

    std::future<std::string> recvFuture = std::async(std::launch::async, [this]() {
        auto recvRes = mServerSocket.Recv();
        if (!recvRes) return std::string{};
        return recvRes.value();
    });

    if (recvFuture.wait_for(std::chrono::milliseconds(2500)) == std::future_status::ready) {
        auto parsed = handler.ParseProtocolString(recvFuture.get());
        return ((parsed.contains(TYPE)) && (parsed[TYPE] == VALIDATE) && (parsed.contains(MESSAGE)) && (parsed[MESSAGE] == SERVER_CONNECTION_ACCEPTED));
    }
    mServerSocket.Close();
    recvFuture.wait();
    return false;
}

void ClientNetworkController::AddInterface(std::shared_ptr<MainInterface> mainInterface) {
    mInterface = mainInterface;
}