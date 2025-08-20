
#include "ConnectPage.h"

#include <thread>
#include <ftxui/component/event.hpp>

#include "MainInterface.h"
#include "ClientNetworkController.h"
#include <shared/shared.h>

using namespace ftxui;

ConnectPage::ConnectPage(MainInterface* mainInterface) : Page{mainInterface}, mConnectButtonLabel{ConnectPage::ConnectLabel} {

    mUsernameField = Input(&mUsernameFieldContent);
    mUsernameField |= CatchEvent([&](Event event) {
        if (event.is_character()) {
            if (event.character()[0] == ' ') {return true;}
            if (mUsernameFieldContent.size() >= 16) {return true;}
        }
        return event == Event::Return; // Prevent newline
    });

    mHostnameField = Input(&mHostnameFieldContent);
    mHostnameField |= CatchEvent([&](Event event) {
        return event == Event::Return;
    });

    mPortField = Input(&mPortFieldContent);
    mPortField |= CatchEvent([&](Event event) {
        if (event.is_character()) {
            if (mPortFieldContent.size() >= 5) { return true; } // max port size
            if (!std::isdigit(event.character()[0])) { return true; }
            int newport = std::stoi(mPortFieldContent+event.character()[0]);
            return (newport < 0 || newport > 65535);
        }
        if (event == Event::Return) { return true; }
        return false;
    });

    mConnectButton = Button(&mConnectButtonLabel, [&] {
        mMainInterface->SetAppState(2);
        return;
        OnConnectButtonPress();
    });

    mExitButton = Button("Close", [&] {
        mMainInterface->Exit();
    });

    mInputContainer = Container::Vertical({
        mUsernameField,
        mHostnameField,
        mPortField,
        mConnectButton,
        mExitButton
    });

    mPageContent = Renderer(mInputContainer, [&] {
        return 
            center(
                hcenter({
                    window(text("Connect To Server"), 
                        hbox({
                            filler() | size(WIDTH, EQUAL, 1),
                            vbox({
                                filler() | size(HEIGHT, EQUAL, 1),
                                hbox({text("Username: "), mUsernameField->Render()}),
                                hbox({text("Hostname: "), mHostnameField->Render()}),
                                hbox({text("Port: "), mPortField->Render() | size(WIDTH, EQUAL, 6)}), // 5 digits for port num
                                separatorDashed(),
                                vbox({
                                    hbox({filler(), mConnectButton->Render(), filler()}),
                                    hbox({filler(), mExitButton->Render(), filler()})
                                }) | center,
                            }) | size(WIDTH, EQUAL, 40),
                            filler() | size(WIDTH, EQUAL, 1)
                        })
                    )
                })
            );
        
    });    
}

void ConnectPage::OnConnectButtonPress() {
    if (mConnecting || mUsernameFieldContent.empty() || mHostnameFieldContent.empty() || mPortFieldContent.empty()) { 
        return; 
    }
    SetConnecting(true);
    if (mMainInterface) {
        mMainInterface->GetNetworkController()->ConnectToServer(mUsernameFieldContent, mHostnameFieldContent, mPortFieldContent);
    }
}

Component ConnectPage::GetPageContent() {
    return mPageContent;
}

void ConnectPage::SetConnecting(bool connecting) {
    mConnecting = connecting;
    mConnectButtonLabel = connecting ? ConnectPage::ConnectingLabel : ConnectPage::ConnectLabel;
}

