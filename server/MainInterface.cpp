#include "MainInterface.h"

#include <sstream>
#include <algorithm>
#include <cctype>
#include <ftxui/component/component.hpp>

using namespace ftxui;

MainInterface::MainInterface(ServerNetworkController* networkController) : mScreen{ScreenInteractive::Fullscreen()}, mLogPage{this}, mExit{mScreen.ExitLoopClosure()} {

}

void MainInterface::OnCommandEnter(const std::string& command) {
    
    mLogPage.AddLogToList({LogType::UserCommand, command});
    std::string commandBase = GetCommandBase(command);

    if (commandBase == "help") {
        OnHelpCommand();
    } 
    else if (commandBase == "exit") {
        OnExitCommand();
    } 
    else if (commandBase == "clear") {
        OnClearCommand();
    } else if (commandBase == "setport") {

    }

    else {
        mLogPage.AddLogToList({LogType::Error, "Invalid command."});
    }

    mLogPage.ClearedTypedCommandField();
}

void MainInterface::OnHelpCommand() {
    mLogPage.AddLogToList({LogType::Info, 
    "help - Display this page.\n"
    "setport {port#} - Set the port number to run the server on\n"
    "start - Start the server with the current configuration.\n"
    "stop - Stop the server.\n"
    "clear - Clear all previous logs\n"
    "exit - Close the program.\n"
    });
}

void MainInterface::OnExitCommand() {
    mLogPage.AddLogToList({LogType::Alert, "Exiting!"});
    Exit();
}

void MainInterface::OnClearCommand() {
    mLogPage.ClearLogList();
}

void MainInterface::OnSetPortCommand(const std::string& command) {
    auto parsedCommand = ParseCommandArguments(command);
}

std::vector<std::string> MainInterface::ParseCommandArguments(const std::string& command) {
    std::istringstream ss{command};
    std::vector<std::string> commands;
    std::string argument;
    while (ss >> argument) {
        commands.push_back(argument);
    }
    return commands;
}

std::string MainInterface::GetCommandBase(const std::string& command) {
    std::string commandcopy = command;
    std::transform(commandcopy.begin(), commandcopy.end(), commandcopy.begin(), [](unsigned char c) {return std::tolower(c); });
    std::istringstream ss{commandcopy};
    std::string first;
    ss >> first;
    return first;
}

void MainInterface::Run() {
    Component logPageContent = mLogPage.GetPageContent();
    
    auto screen_renderer = Renderer(logPageContent, [&] {
        return logPageContent->Render();
    });

    mScreen.Loop(screen_renderer);
}