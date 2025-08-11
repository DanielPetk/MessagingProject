#pragma once

#include <atomic>
#include <future>
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
    ftxui::Component mExitButton;
    ftxui::Component mInputContainer;
    ftxui::Component mPageContent;

    void OnConnectButtonPress();
    
public:
    
    // UI -> Network on button
    std::function<void(const std::string&, const std::string& , const std::string&)> mAttemptConnection;    

    ConnectPage(ClientApp*);
    ftxui::Component GetPageContent() override;    
    void SetConnecting(bool connecting);
};