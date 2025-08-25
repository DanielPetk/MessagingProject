#include <format>
#include <ranges>
#include <iostream>

#include "ChatPage.h"
#include "MainInterface.h"

using namespace ftxui;

ChatPage::ChatPage(MainInterface* mainInterface) : Page(mainInterface) {

    mDisplayedMessages.reserve(ChatPage::MAX_MESSAGE_AMOUNT);

    mSendButton = Button("Send Message", [&] {
        SendMessageHelper();
    });

    mLeaveButton = Button("Leave Room", [&] {
        mMainInterface->OnLeaveRoom();
    });

    mTypedMessageInput = Input(&mTypedMessageContent, {
        .placeholder = "Type your message here..."
    });

    mTypedMessageInput |= CatchEvent([&](Event event) {
        if (event.is_character()) {
            if (mTypedMessageContent.size() >= ChatPage::MAX_MESSAGE_SIZE) { return true; } // max message size
        }
        if (event == Event::Return) { 
            SendMessageHelper();
            return true;
        }
        return false;
    });

    mInputLayout = Container::Horizontal({
        mTypedMessageInput, mSendButton, mLeaveButton
    });

    mPageContent = Renderer(mInputLayout, [&] {
        mDisplayedMessages.clear();
        for (auto &msg : mMessages) {
            mDisplayedMessages.emplace_back(Format(msg));
        }
        
        if (mDisplayedMessages.size()) {
            mDisplayedMessages.at(mDisplayedMessages.size() -1) = focus(mDisplayedMessages.at(mDisplayedMessages.size() -1)); // Focus latest chat so it scrolls
        }

        return 
            center(
                hcenter({
                    window(text("Chat"),
                        vbox(
                            vbox(filler(), frame(vbox(mDisplayedMessages))) | flex,
                            hbox(
                                vbox(
                                    filler() | size(HEIGHT, EQUAL, 1),
                                    mTypedMessageInput->Render(),
                                    filler() | size(HEIGHT, EQUAL, 1)
                                ) | flex,
                                mSendButton->Render(), 
                                mLeaveButton->Render()
                            ) | size(HEIGHT, EQUAL, 3) | border 
                        ) | size(HEIGHT, EQUAL, 50) | size(WIDTH, EQUAL, 120)
                    )
                })
            );
    });
};

Element ChatPage::Format(const Message& message) {
    Element messageElem = paragraph(std::format("[{}] {}", message.mUsername, message.mMessage));
    if (message.mSentByThisClient) {
        messageElem |= color(Color::Yellow1);
    }
    return messageElem;
}
void ChatPage::AddMessageToList(const Message& message) {
    if (mMessages.size() >= ChatPage::MAX_MESSAGE_AMOUNT) {
        mMessages.pop_front();
    }
    mMessages.emplace_back(message);
}

void ChatPage::SendMessageHelper() {
    if (mTypedMessageContent.size()) {  
        mMainInterface->OnSendMessage(mTypedMessageContent);
    }
}

Component ChatPage::GetPageContent() {
    return mPageContent;
}

void ChatPage::ClearTypedMessageField() {
    mTypedMessageContent.clear();
}

void ChatPage::ClearMessageHistory() {
    mMessages.clear();
}