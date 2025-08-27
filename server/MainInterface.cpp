#include "MainInterface.h"

#include <ftxui/component/component.hpp>

using namespace ftxui;

MainInterface::MainInterface(ServerNetworkController* networkController) : mScreen{ScreenInteractive::FitComponent()}, mLogPage{this}, mExit{mScreen.ExitLoopClosure()} {

}

void MainInterface::Run() {
    Component logPageContent = mLogPage.GetPageContent();
    
    auto screen_renderer = Renderer(logPageContent, [&] {
        return logPageContent->Render();
    });

    mScreen.Loop(screen_renderer);
}