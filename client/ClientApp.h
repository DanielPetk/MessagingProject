#pragma once

#include <memory>

#include "ClientNetworkController.h"
#include "MainInterface.h"

class ClientApp {

    ClientNetworkController mNetworkController;
    std::shared_ptr<MainInterface> mMainInterface = nullptr;

public:

    ClientApp();
    void Run();
};