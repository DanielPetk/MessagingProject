
#include <ftxui/component/event.hpp>
#include <thread>
#include <atomic>
#include <shared/shared.h>
#include "ConnectPage.h"
#include "MainUI.h"
using namespace ftxui;

ConnectPage::ConnectPage(MainUI* mainUI) : Page{mainUI}, mConnectButtonLabel{"Connect"} {

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
    
    if (mConnecting || mUsernameFieldContent.empty() || mHostnameFieldContent.empty() || mPortFieldContent.empty()) { return; }
    SetConnecting(true);

    // New thread to avoid blocking UI thread which blocks other actions
    std::thread([&] {
        SOCKET serverSocket = ConnectToServer();
        if (serverSocket == SOCKET_ERROR || serverSocket == INVALID_SOCKET){
            mMainUI->SetAppState(1);
        }
        else {
            SendClientInfo(serverSocket);
            mMainUI->SetAppState(2);
        }
        SetConnecting(false);   
        mMainUI->GetScreen().RequestAnimationFrame();
    }).detach();
    
}

bool ConnectPage::SendClientInfo(SOCKET serverSocket) {
    std::string initialMessage = APP_IDENTIFIER + DELIM + mUsernameFieldContent;
    send(serverSocket, initialMessage.data(), initialMessage.size(), 0);
    return true;
};

SOCKET ConnectPage::ConnectToServer() {
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(std::stoi(mPortFieldContent));
    
    hostent* he = gethostbyname(mHostnameFieldContent.c_str());
    if (!he) {
        return SOCKET_ERROR;
    }
    memcpy(&server_addr.sin_addr, he->h_addr, he->h_length);

    // Create socket
    SOCKET ssock = socket( AF_INET, SOCK_STREAM, 0 );
    if(ssock == INVALID_SOCKET){
        return SOCKET_ERROR;
    }

    // Connect to server 
    if (connect(ssock, (struct sockaddr*) &server_addr, sizeof(server_addr)) == SOCKET_ERROR){
        closesocket(ssock);
        return SOCKET_ERROR;
    }    

    return ssock;
}


Component ConnectPage::GetPageContent() {
    return mPageContent;
}

void ConnectPage::SetConnecting(bool connecting) {
    mConnecting = connecting;
    mConnectButtonLabel = connecting ? "Connecting..." : "Connect";
}
