#pragma once

#include <atomic>
#include <string>
#include <winsock2.h>

#include "Page.h"

class ClientApp;

class ConnectPage : public Page {
    static constexpr const char* ConnectLabel = "Connect";
    static constexpr const char* ConnectingLabel = "Connecting...";

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
    ConnectPage(ClientApp*);
    ftxui::Component GetPageContent() override;    
};