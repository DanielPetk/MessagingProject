#pragma once

#include <list>

#include "ClientNetworkController.h"
#include <shared/Page.h>


class MainInterface;

class ChatPage : public Page {

    static constexpr int MAX_MESSAGE_SIZE = 200;
    static constexpr int MAX_MESSAGE_AMOUNT = 80; 

    std::list<Message> mMessages;
    std::vector<ftxui::Element> mDisplayedMessages;
    std::string mTypedMessageContent;
    
    ftxui::Component mSendButton;
    ftxui::Component mLeaveButton;
    ftxui::Component mTypedMessageInput;
    ftxui::Component mInputLayout;
    ftxui::Component mPageContent;
    
    ftxui::Element Format(const Message& message);
    void SendMessageHelper();
public:

    void AddMessageToList(const Message& message);
    void ClearTypedMessageField();
    void ClearMessageHistory();

    ChatPage(MainInterface*);
    ftxui::Component GetPageContent() override;
};