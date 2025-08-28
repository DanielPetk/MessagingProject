#pragma once

#include "ServerNetworkController.h"
#include "LogPage.h"

#include <ftxui/component/screen_interactive.hpp>

enum class LogType {
    UserCommand,
    Info,
    Alert,
    Error
};

struct LogEntry {
    LogEntry(LogType type, const std::string& message) : mType{type}, mMessage{message} {}
    LogType mType;
    std::string mMessage;
};

class MainInterface {

    // The server's network controller to call ui stuff on
    ServerNetworkController* mNetworkController = nullptr;

    /// The main ftxui
    ftxui::ScreenInteractive mScreen; 
    
    /// Callable closure to exit the UI loop
    ftxui::Closure mExit;

    // UI LOG PAGE
    LogPage mLogPage;

    std::string GetCommandBase(std::string command);

    void OnHelpCommand();
    void OnExitCommand();
    void OnClearCommand();
public:

    // Called from UI thread
    void OnCommandEnter(const std::string& command);

    MainInterface(ServerNetworkController* networkController);
    void Run();
    void Exit() { mExit(); };
};