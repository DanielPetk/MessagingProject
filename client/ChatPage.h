#pragma once

#include <shared/Page.h>

class MainInterface;

class ChatPage : public Page {

public:
    ChatPage(MainInterface*);
    ftxui::Component GetPageContent() override;
};