#include "ClientApp.h"

ClientApp::ClientApp() {
    mMainInterface = std::make_shared<MainInterface>(&mNetworkController);
    mNetworkController.AddInterface(mMainInterface);
}

void ClientApp::Run() {
    mMainInterface->Run();
}