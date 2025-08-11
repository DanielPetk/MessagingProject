#include "ClientApp.h"

#include <iostream>

#include <ftxui/component/component.hpp>

#include "ConnectErrorPage.h"
#include "ConnectPage.h"


using namespace ftxui;

ClientApp::ClientApp() : mConnectPage{this}, mConnectErrorPage{this}, mChatPage{this}, mScreen{ScreenInteractive::Fullscreen()}, mExit{mScreen.ExitLoopClosure()} {
    
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
    mExit();
}

void ClientApp::SetAppState(int appState) {
    mAppState = appState; 
}

ScreenInteractive& ClientApp::GetScreen() {
    return mScreen;
}

ClientNetworkController& ClientApp::GetController() {
    return mClientNetworkController;
}