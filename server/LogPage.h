#pragma once

#include <shared/Page.h>

class LogPage : public Page {

public:
    LogPage(MainInterface* mainInterface);
    ftxui::Component GetPageContent() override;
};