#include "MainInterface.h"

#include <iostream>

#include <ftxui/component/component.hpp>

#include "ClientNetworkController.h"
#include "ConnectErrorPage.h"
#include "ConnectPage.h"


using namespace ftxui;

MainInterface::MainInterface(ClientNetworkController* networkController) : mConnectPage{this}, mConnectErrorPage{this}, mChatPage{this}, 
    mScreen{ScreenInteractive::Fullscreen()}, mExit{mScreen.ExitLoopClosure()}, mNetworkController{networkController} {
}

void MainInterface::Run() {
    Component connectPageContent = mConnectPage.GetPageContent();
    Component connectErrorPageContent = mConnectErrorPage.GetPageContent();
    Component chatPageContent = mChatPage.GetPageContent();


    Component mainContainer = Container::Tab({
        connectPageContent,
        connectErrorPageContent,
        chatPageContent,
    }, &mAppState);
    
    auto screen_renderer = Renderer(mainContainer, [&] {
        switch(mAppState){
            case 0: return connectPageContent->Render(); 
            case 1: return connectErrorPageContent->Render();
            case 2: return chatPageContent->Render();
        }
        return text("Invalid State");
    });
    
    mScreen.Loop(screen_renderer);
}

// Called from other threads
void MainInterface::OnConnectionError() {
    mScreen.Post([this] {
        mConnectPage.SetConnecting(false);
        mNetworkController->SetRunning(false);
        SetAppState(1);
    });
    mScreen.RequestAnimationFrame();
}

void MainInterface::OnConnectionSuccess() {
    mScreen.Post([this] {
        mConnectPage.SetConnecting(false);
        mChatPage.ClearMessageHistory();
        mNetworkController->StartReceiveMessageLoop();
        SetAppState(2);
    });
    mScreen.RequestAnimationFrame();
}

void MainInterface::OnLoopError() {
    mScreen.Post([this] {
        mNetworkController->CloseServerConnection();
        SetAppState(1);
    });
    mScreen.RequestAnimationFrame();
}


void MainInterface::OnReceivedMessage(const Message& message) {
    mScreen.Post([message, this] {
        mChatPage.AddMessageToList(message);
    });
    mScreen.RequestAnimationFrame();
}

void MainInterface::OnLeaveRoom() {
    mNetworkController->CloseServerConnection();
    SetAppState(0);
}

// Called from UI threads
void MainInterface::OnConnectButtonPress(const std::string& username, const std::string& host, const std::string& port) {
    mConnectPage.SetConnecting(true);
    mNetworkController->ConnectToServer(username, host, port);
}

void MainInterface::OnSendMessage(const std::string& message) { 
    bool sendSuccess = mNetworkController->SendServerMessage(message);
    if (sendSuccess) {
        mChatPage.AddMessageToList({"You", message, MessageType::Sent});
    }
    else {
        mChatPage.AddMessageToList({"ERROR", "Failed to send message", MessageType::Error});
    }
    mChatPage.ClearTypedMessageField();
}