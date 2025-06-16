#pragma once
#include <winsock2.h>
#include <string>
#include "Page.h"

class MainUI;

class ConnectPage : public Page {
    std::string mUsernameFieldContent;
    std::string mHostnameFieldContent;
    std::string mPortFieldContent;
    std::string mConnectButtonLabel;
    std::atomic<bool> mConnecting = false;

    ftxui::Component mUsernameField;
    ftxui::Component mHostnameField;
    ftxui::Component mPortField;
    ftxui::Component mConnectButton;
    ftxui::Component mInputContainer;
    ftxui::Component mPageContent;

    void OnConnectButtonPress();
    SOCKET ConnectToServer();
    bool SendClientInfo(SOCKET serverSocket);
    void SetConnecting(bool);
public:
    ConnectPage(MainUI*);
    ftxui::Component GetPageContent() override;    
};