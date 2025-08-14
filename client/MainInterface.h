#pragma once

#include <mutex> 
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

    // Current screen being displayed and mutex for it;
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

    void Run();
    void Exit() { mExit(); };
    void SetAppState(int appState) { mAppState = appState; };
    ClientNetworkController* GetNetworkController() { return mNetworkController; }
    ftxui::ScreenInteractive& GetScreen() { return mScreen; };

};