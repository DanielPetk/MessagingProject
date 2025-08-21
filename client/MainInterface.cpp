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
    mScreen.Post([&] {
        mConnectPage.SetConnecting(false);
        SetAppState(1);
    });
    mScreen.RequestAnimationFrame();
}

void MainInterface::OnConnectionSuccess() {
    mScreen.Post([&] {
        mConnectPage.SetConnecting(false);
        SetAppState(2);
    });
    mScreen.RequestAnimationFrame();
}

void MainInterface::OnLeaveRoom() {
    SetAppState(0);
}

// Called from UI threads
void MainInterface::OnConnectButtonPress(const std::string& username, const std::string& host, const std::string& port) {
    mConnectPage.SetConnecting(true);
    mNetworkController->ConnectToServer(username, host, port);
}

void MainInterface::OnSendMessage(const std::string& message) { 
    mChatPage.AddMessageToList({"You", message, true});
    mChatPage.ClearTypedMessageField();
}