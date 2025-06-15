#pragma once
#include <ftxui/component/screen_interactive.hpp>
#include "ConnectPage.h"
#include "ConnectErrorPage.h"

class MainUI {
    int mAppState = 0;
    ftxui::ScreenInteractive mScreen; 
    ConnectPage mConnectPage;
    ConnectErrorPage mConnectErrorPage;
public:
    MainUI();
    void Run();
    void Exit();
    void SetAppState(int);
    ftxui::ScreenInteractive& GetScreen();
};