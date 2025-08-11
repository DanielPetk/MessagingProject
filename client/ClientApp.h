#pragma once

#include <atomic>
#include <ftxui/component/screen_interactive.hpp>

#include "ChatPage.h"
#include "ClientNetworkController.h"
#include "ConnectErrorPage.h"
#include "ConnectPage.h"

class ClientApp {
    ftxui::ScreenInteractive mScreen; 
    ftxui::Closure mExit;
    int mAppState = 0;
    
    // Whether the signals should actually do something
    std::atomic<bool> mPropogateEvents = false;

    // UI -> Network Signals

    // Network -> UI Signals

    // Network stuff
    ClientNetworkController mClientNetworkController;
    
    // UI Pages
    ConnectPage mConnectPage;
    ConnectErrorPage mConnectErrorPage;
    ChatPage mChatPage;
public:
    ClientApp();
    void Run();
    void Exit();
    void SetAppState(int);
    ftxui::ScreenInteractive& GetScreen();
    ClientNetworkController& GetController();
};