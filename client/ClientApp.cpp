#include <ftxui/component/component.hpp>
#include "ClientApp.h"
#include "ConnectPage.h"
#include "ConnectErrorPage.h"

#include <iostream>

using namespace ftxui;

ClientApp::ClientApp() : mConnectPage{this}, mConnectErrorPage{this}, mChatPage{this}, mScreen{ScreenInteractive::Fullscreen()} {
    
}

void ClientApp::Run() {
    Component connectPageContent = mConnectPage.GetPageContent();
    Component connectErrorPageContent = mConnectErrorPage.GetPageContent();
    Component chatPageContent = mChatPage.GetPageContent();

    Component mainContainer = Container::Tab({
        connectPageContent,
        connectErrorPageContent,
        chatPageContent,
    }, &mAppState);
    
    auto screen_renderer = Renderer(mainContainer, [&] {
        switch(mAppState){
            case 0: return connectPageContent->Render(); 
            case 1: return connectErrorPageContent->Render();
            case 2: return chatPageContent->Render();
        }
        return text("Invalid State");
    });
        
    mScreen.Loop(screen_renderer);
}

void ClientApp::Exit() {
    mScreen.ExitLoopClosure()();
}

void ClientApp::SetAppState(int appState) {
    mAppState = appState; 
}

ScreenInteractive& ClientApp::GetScreen() {
    return mScreen;
}