#include "MainInterface.h"

#include <ftxui/component/component.hpp>

using namespace ftxui;

MainInterface::MainInterface(ServerNetworkController* networkController) : mScreen{ScreenInteractive::TerminalOutput()}, mLogPage{this}, mExit{mScreen.ExitLoopClosure()} {

}

void MainInterface::OnCommandEnter(const std::string& command) {
    mLogPage.ClearedTypedCommandField();
}

void MainInterface::Run() {
    Component logPageContent = mLogPage.GetPageContent();
    
    auto screen_renderer = Renderer(logPageContent, [&] {
        return logPageContent->Render();
    });

    mScreen.Loop(screen_renderer);
}