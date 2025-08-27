#pragma once

#include <memory>

class MainInterface;

class ServerNetworkController {

    std::shared_ptr<MainInterface> mInterface = nullptr;

public:
    void AddInterface(std::shared_ptr<MainInterface> mainInterface) {mInterface = mainInterface;}

};