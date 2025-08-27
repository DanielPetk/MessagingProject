#pragma once

#include <string>
#include <shared/Page.h>

class LogPage : public Page {

    static constexpr int MAX_COMMAND_SIZE = 300;

    std::string mCommandInputContent;

    ftxui::Component mCommandInput;
    ftxui::Component mPageContent;

    ftxui::Element GetServerHeader();
    void EnterCommandHelper();
    
public:
    LogPage(MainInterface* mainInterface);
    void ClearedTypedCommandField();
    ftxui::Component GetPageContent() override;
};