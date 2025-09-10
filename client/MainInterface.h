#pragma once

#include <mutex> 
#include <memory>
#include <ftxui/component/screen_interactive.hpp>

#include "ChatPage.h"
#include "ConnectErrorPage.h"
#include "ConnectPage.h"

class ClientNetworkController;

class MainInterface {
    
    /// The network controller used to call functions on
    ClientNetworkController* mNetworkController = nullptr;

    /// The main ftxui
    ftxui::ScreenInteractive mScreen; 
    
    /// Callable closure to exit the UI loop
    ftxui::Closure mExit;

    // Current screen being displayed
    int mAppState = 0;
    
    // UI Pages
    ConnectPage mConnectPage;
    ConnectErrorPage mConnectErrorPage;
    ChatPage mChatPage;

public:
    MainInterface(ClientNetworkController* networkController);
   
    // Callbacks for different ui events that will be called from other threads
    void OnConnectionError();
    void OnConnectionSuccess();
    void OnLoopError();

    // Called from UI Threads
    void OnConnectButtonPress(const std::string& username, const std::string& host, const std::string& port);
    void OnLeaveRoom();
    void OnSendMessage(const std::string& message);
    void OnReceivedMessage(const Message& message);

    void Run();
    void Exit() { mExit(); };
    void SetAppState(int appState) { mAppState = appState; };
};