#pragma once

#include <memory>
#include "ServerNetworkController.h"
#include "MainInterface.h"

class ServerApp {

    ServerNetworkController mNetworkController;
    std::shared_ptr<MainInterface> mMainInterface = nullptr;

public:
    ServerApp();
    void Run();
};