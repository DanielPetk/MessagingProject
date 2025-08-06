
#include "ConnectPage.h"

#include <thread>
#include <ftxui/component/event.hpp>

#include "ClientApp.h"
#include <shared/shared.h>

using namespace ftxui;

ConnectPage::ConnectPage(ClientApp* clientApp) : Page{clientApp}, mConnectButtonLabel{ConnectPage::ConnectLabel} {

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
        OnConnectButtonPress();
    });
      
    mInputContainer = Container::Vertical({
        mUsernameField,
        mHostnameField,
        mPortField,
        mConnectButton
    });

    mPageContent = Renderer(mInputContainer, [&] {
        return window(text("Connect To Server"),
            center(
                hcenter({
                    vbox({
                        hbox({text("Username: "), mUsernameField->Render()}),
                        hbox({text("Hostname: "), mHostnameField->Render()}),
                        hbox({text("Port: "), mPortField->Render() | size(WIDTH, EQUAL, 6)}), // 5 digits for port num
                        separatorDashed(),
                        hbox({filler(), mConnectButton->Render(), filler()})
                    }) | size(WIDTH, EQUAL, 30), 
                })
            )
        );
    });    
}

void ConnectPage::OnConnectButtonPress() {
    
    if (mConnecting || mUsernameFieldContent.empty() || mHostnameFieldContent.empty() || mPortFieldContent.empty()) { 
        return; 
    }
    
    mConnectButtonLabel = ConnectPage::ConnectingLabel;

    // New thread to avoid blocking UI thread which blocks other actions
    std::thread([&] {
        if ( mClientApp->GetController().ConnectToServer(mUsernameFieldContent, mHostnameFieldContent, mPortFieldContent) ){
            mClientApp->SetAppState(2);
        }
        else {
            mClientApp->SetAppState(1);
        }
        mConnectButtonLabel = ConnectPage::ConnectLabel;
        mClientApp->GetScreen().RequestAnimationFrame();
    }).detach();
    
}

Component ConnectPage::GetPageContent() {
    return mPageContent;
}

