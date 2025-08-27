#include "ServerApp.h"

ServerApp::ServerApp() {
    mMainInterface = std::make_shared<MainInterface>(&mNetworkController);
    mNetworkController.AddInterface(mMainInterface);
}

void ServerApp::Run() {
    mMainInterface->Run();
}