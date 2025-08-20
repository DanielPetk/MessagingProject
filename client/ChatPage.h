#pragma once

#include <vector>

#include "ClientNetworkController.h"
#include <shared/Page.h>


class MainInterface;

class ChatPage : public Page {

    std::vector<Message> mMessages = {{"John", "This is a test message"},{"John", "This is a test message"},{"John", "This is a test message"},{"John", "This is a test message"},{"John", "This is a test message"},{"John", "This is a test message"},{"John", "This is a test message"},{"John", "This is a test message"},{"John", "This is a test message"},{"John", "This is a test message"},{"John", "This is a test message"},{"John", "This is a test message"},{"John", "This is a test message"},{"John", "This is a test message"},{"John", "This is a test message"},{"John", "This is a test message"},{"John", "This is a test message"},{"John", "This is a test message"},{"John", "This is a test message"},{"John", "This is a test message"},{"John", "This is a test message"},{"John", "This is a test message"},{"John", "This is a test message"},{"John", "This is a test message"},{"John", "This is a test message"},{"John", "This is a test message"},{"John", "This is a test message"},{"John", "This is a test message"},{"John", "This is a test message"},{"John", "This is a test message"},{"John", "This is a test message"},{"John", "This is a test message"},{"John", "This is a test message"},{"John", "This is a test message"},{"John", "This is a test message"},{"John", "This is a test message"},{"John", "This is a test message"},{"John", "This is a test message"},{"John", "This is a test message"},{"John", "This is a test message"},{"John", "This is a test message"},{"John", "This is a test message"},{"John", "This is a test message"},{"John", "This is a test message"},{"John", "This is a test message"},{"John", "This is a test message"},{"John", "This is a test message"},{"John", "This is a test message"},{"John", "This is a test message"},{"John", "This is a test message"},{"John", "This is a test message"},{"John", "This is a test message"},{"John", "This is a test message"},{"John", "This is a test message"},{"John", "This is a test message"},{"John", "This is a test message"},{"John", "This is a test message"},{"John", "This is a test message"},{"John", "This is a test message"},{"John", "This is a test message"},{"John", "This is a test message"},{"John", "This is a test message"},{"John", "This is a test message"},{"John", "This is a test message"},{"John", "This is a test message"},};
    std::vector<ftxui::Element> mDisplayedMessages;

    std::string mTypedMessageContent;

    ftxui::Component mSendButton;
    ftxui::Component mLeaveButton;
    ftxui::Component mTypedMessageInput;
    ftxui::Component mInputLayout;
    ftxui::Component mPageContent;

    ftxui::Element Format(const Message& message);
public:
    ChatPage(MainInterface*);
    ftxui::Component GetPageContent() override;
};