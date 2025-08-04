#pragma once

#include "Page.h"

class ClientApp;

class ChatPage : public Page {

public:
    ChatPage(ClientApp*);
    ftxui::Component GetPageContent() override;
};