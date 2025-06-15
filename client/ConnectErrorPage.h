#pragma once
#include "Page.h"

class ConnectErrorPage : public Page {

    ftxui::Component mOkayButton;
    ftxui::Component mPageContent;

public:
    ConnectErrorPage(MainUI*);
    ftxui::Component GetPageContent() override;
};