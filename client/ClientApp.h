#pragma once

#include <ftxui/component/screen_interactive.hpp>

#include "ChatPage.h"
#include "ConnectErrorPage.h"
#include "ConnectPage.h"

class ClientApp {
    int mAppState = 0;
    ftxui::ScreenInteractive mScreen; 
    ConnectPage mConnectPage;
    ConnectErrorPage mConnectErrorPage;
    ChatPage mChatPage;
public:
    ClientApp();
    void Run();
    void Exit();
    void SetAppState(int);
    ftxui::ScreenInteractive& GetScreen();
};