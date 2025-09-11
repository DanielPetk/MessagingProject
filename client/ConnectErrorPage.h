#pragma once

#include <shared/Page.h>

class ConnectErrorPage : public Page {

    ftxui::Component mOkayButton;
    ftxui::Component mPageContent;

public:
    ConnectErrorPage(MainInterface*, const std::string& errorMessage);
    ftxui::Component GetPageContent() override;
};