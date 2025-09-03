#include "MainInterface.h"

#include <sstream>
#include <algorithm>
#include <cctype>
#include <ftxui/component/component.hpp>

using namespace ftxui;

const std::string INVALID_SYNTAX = "Invalid number of arguments. Type 'help' for information on how to use this command.";
const std::string INVALID_COMMAND = "Invalid command. Type 'help' for a list of commands.";

MainInterface::MainInterface(ServerNetworkController* networkController) : mScreen{ScreenInteractive::Fullscreen()}, mLogPage{this}, mExit{mScreen.ExitLoopClosure()}, mNetworkController{networkController} {}

void MainInterface::OnCommandEnter(const std::string& command) {
    
    mLogPage.AddLogToList({LogType::UserCommand, command});
    std::string commandBase = GetCommandBase(command);

    if (commandBase == "help") {
        OnHelpCommand();
    } else if (commandBase == "exit") {
        OnExitCommand();
    } else if (commandBase == "clear") {
        OnClearCommand();
    } else if (commandBase == "setport") {
        OnSetPortCommand(command);
    } else if (commandBase == "start") {
        OnStartCommand();
    } else {
        mLogPage.AddLogToList({LogType::Error, INVALID_COMMAND});
    }

    mLogPage.ClearedTypedCommandField();
}

void MainInterface::OnHelpCommand() {
    mLogPage.AddLogToList({LogType::Info, 
    "help - Display this page.\n"
    "setport {port#} - Set the port number to run the server on\n"
    "start - Start the server with the current configuration.\n"
    "stop - Stop the server.\n"
    "clear - Clear all previous logs.\n"
    "exit - Close the program.\n"
    });
}

void MainInterface::OnStartCommand() {
    if (mNetworkController->GetRunning()) {
        mLogPage.AddLogToList({LogType::Error, "Server already running."});
        return;
    }
    auto host = mNetworkController->StartListening();
    if (host) {
        mLogPage.AddLogToList({LogType::Alert, std::format("{} listening on port {}.", host.value(), mNetworkController->GetPort())});
    }
    else {
        mLogPage.AddLogToList({LogType::Error, "Failed to start server."});
    }
}

void MainInterface::OnExitCommand() {
    mLogPage.AddLogToList({LogType::Alert, "Exiting."});
    Exit();
}

void MainInterface::OnClearCommand() {
    mLogPage.ClearLogList();
}

void MainInterface::OnSetPortCommand(const std::string& command) {
    
    static const std::string INVALID_PORT = "Port must be an integer from 1 to 65535.";

    auto parsedCommand = ParseCommandArguments(command);
    if (parsedCommand.size() != 2 || parsedCommand.at(1).size() == 0) {
        mLogPage.AddLogToList({LogType::Error, INVALID_SYNTAX});
        return;
    }

    std::string potentialport = parsedCommand.at(1);
    bool portnumeric = std::all_of(potentialport.begin(), potentialport.end(), [](unsigned char c) {
        return std::isdigit(c);
    });

    if (!portnumeric) {
        mLogPage.AddLogToList({LogType::Error, INVALID_PORT});
        return;
    }

    int port;
    try {
        port = std::stoi(potentialport);
    } catch (...) {
        mLogPage.AddLogToList({LogType::Error, INVALID_PORT});
        return;
    }

    if (port >=1 && port <= 65535){
        mNetworkController->SetPort(port);
        mLogPage.AddLogToList({LogType::Alert, std::format("Port successfully updated to {}. Changes will be in effect next time the server restarts.", port)});
    }
    else {
        mLogPage.AddLogToList({LogType::Error, INVALID_PORT});
    }

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