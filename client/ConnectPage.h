#pragma once

#include <atomic>
#include <future>
#include <string>
#include <winsock2.h>

#include <shared/Page.h>

class MainInterface;

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

    ftxui::Component testbutton;
    
public:

    ConnectPage(MainInterface*);
    ftxui::Component GetPageContent() override;    
    void SetConnecting(bool connecting);
};