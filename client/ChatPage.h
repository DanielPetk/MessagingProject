#pragma once
#include "Page.h"

class MainUI;

class ChatPage : public Page {

public:
    ChatPage(MainUI*);
    ftxui::Component GetPageContent() override;
};