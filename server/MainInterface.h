#pragma once

#include "ServerNetworkController.h"
#include "LogPage.h"

#include <ftxui/component/screen_interactive.hpp>

class MainInterface {

    // The server's network controller to call ui stuff on
    ServerNetworkController* mNetworkController = nullptr;

    /// The main ftxui
    ftxui::ScreenInteractive mScreen; 
    
    /// Callable closure to exit the UI loop
    ftxui::Closure mExit;

    // UI LOG PAGE
    LogPage mLogPage;

public:
    MainInterface(ServerNetworkController* networkController);
    void Run();
    void Exit();
};