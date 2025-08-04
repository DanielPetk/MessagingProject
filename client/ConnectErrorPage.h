#pragma once

#include "Page.h"

class ConnectErrorPage : public Page {

    ftxui::Component mOkayButton;
    ftxui::Component mPageContent;

public:
    ConnectErrorPage(ClientApp*);
    ftxui::Component GetPageContent() override;
};