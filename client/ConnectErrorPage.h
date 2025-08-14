#pragma once

#include <shared/Page.h>

class ConnectErrorPage : public Page {

    ftxui::Component mOkayButton;
    ftxui::Component mPageContent;

public:
    ConnectErrorPage(MainInterface*);
    ftxui::Component GetPageContent() override;
};