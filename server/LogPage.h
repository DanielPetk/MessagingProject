#pragma once

#include <list>
#include <vector>
#include <string>
#include <shared/Page.h>

struct LogEntry;

class LogPage : public Page {

    static constexpr int MAX_COMMAND_SIZE = 300;
    static constexpr int MAX_LOG_AMOUNT = 1000;
    
    std::list<LogEntry> mLogEntries;
    std::vector<ftxui::Element> mDisplayedLogs;

    std::string mCommandInputContent;

    ftxui::Component mCommandInput;
    ftxui::Component mPageContent;

    ftxui::Element GetServerHeader();
    void EnterCommandHelper();
    ftxui::Element Format(const LogEntry& entry);
    
public:
    LogPage(MainInterface* mainInterface);
    void AddLogToList(const LogEntry& entry);
    void ClearLogList();
    void ClearedTypedCommandField();
    ftxui::Component GetPageContent() override;
};