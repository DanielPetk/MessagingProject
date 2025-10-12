#include <chrono>
#include <iostream>

#include "ClientNetworkController.h"
#include "MainInterface.h"
#include <shared/shared.h>
#include <shared/protocol/ProtocolHandler.h>


ClientNetworkController::~ClientNetworkController() {
    CloseServerConnection(true);
}

void ClientNetworkController::StartReceiveMessageLoop() {
    if (!mInterface || mLoopRecv) { return; }
    mLoopRecv = true;
    ProtocolHandler handler;

    mReceivingMessageThread = std::jthread([=, this] {
        while (mLoopRecv) {
            auto recvRes = mServerSocket.Recv();
            if (!recvRes || recvRes.value() == "") {
                if (mShowLoopError) {
                    mInterface->OnLoopError();
                }
                return;
            }

            auto parsed = handler.ParseProtocolString(recvRes.value());
            if ((parsed.contains(TYPE)) && (parsed[TYPE] == CHAT) && (parsed.contains(MESSAGE)) && (parsed.contains(USERNAME))) {

            }
            mInterface->OnReceivedMessage({parsed[USERNAME], parsed[MESSAGE]});
        }
    });
}

void ClientNetworkController::ConnectToServer(const std::string& username, const std::string& host, const std::string& port) {    
    if (!mInterface || mRunning) { return; }
    mRunning = true;

    mConnectingThread = std::jthread([=, this] {    

        // Create socket
        mServerSocket = Socket{SocketMode::TCP};
        
	if (!mServerSocket.SetSendTimeout(2500)) {
            mServerSocket.Close();
	    mInterface->OnConnectionError();
            return;
        }
	
	int portnum;
        try {
            portnum = std::stoi(port);
        } catch (...) {
            mServerSocket.Close();
            mInterface->OnConnectionError();
            return;
        }
        // Connect to server 
        if (!mServerSocket.Connect(host, portnum)) {
            mServerSocket.Close();
            mInterface->OnConnectionError();
            return;
        }
        
        if(ValidateServer(username)) {
            mUsername = username;
            mInterface->OnConnectionSuccess();
        }
        else {
            mServerSocket.Close();
            mInterface->OnConnectionError();
        }
    });   
}

void ClientNetworkController::CloseServerConnection(bool suppressErrors) {
    if (suppressErrors) {
        mShowLoopError = false;
    }
    std::lock_guard<std::mutex> m{mCleanupMutex};
    mLoopRecv = false;
    mServerSocket.Close();
    if (mConnectingThread.joinable()) { mConnectingThread.join(); }
    if (mReceivingMessageThread.joinable()) {mReceivingMessageThread.join(); }
    mShowLoopError = true;
    mRunning = false;
}

bool ClientNetworkController::SendServerMessage(const std::string& message) {

    ProtocolHandler handler;
    std::string protocolMessage = handler.CreateProtocolString({
        {TYPE, CHAT},
        {MESSAGE, message},
        {USERNAME, mUsername}
    });

    auto sendRes = mServerSocket.Send(protocolMessage);
    return !(!sendRes || sendRes.value() == 0);
}

bool ClientNetworkController::ValidateServer(const std::string& username) {
    ProtocolHandler handler;        
    std::string validationMessage = handler.CreateProtocolString({{TYPE, VALIDATE}, {MESSAGE, APP_IDENTIFIER}, {USERNAME, username}});
    auto sendRes = mServerSocket.Send(validationMessage);
    
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
