#include <format>

#include "ChatPage.h"
#include "MainInterface.h"

using namespace ftxui;

ChatPage::ChatPage(MainInterface* mainInterface) : Page(mainInterface) {

    mSendButton = Button("Send Message", [&] {
        // TODO FILL THIS IN
    });

    mLeaveButton = Button("Leave Room", [&] {
        // TODO FILL THIS IN
    });

    mTypedMessageInput = Input(&mTypedMessageContent, {
        .placeholder = "Type your message here..."
    });

    // mTypedMessageInput |= CatchEvent([&](Event event) {
    //     if (event.is_character()) {
    //         if (mTypedMessageContent.size() >= 5) { return true; } // max port size
    //         if (!std::isdigit(event.character()[0])) { return true; }

    //     }
    //     if (event == Event::Return) { return true; }
    //     return false;
    // });

    mInputLayout = Container::Horizontal({
        mTypedMessageInput, mSendButton, mLeaveButton
    });

    mPageContent = Renderer(mInputLayout, [&] {
        mDisplayedMessages.clear();
        mDisplayedMessages.reserve(mMessages.size());
        for (auto &msg : mMessages) {
            mDisplayedMessages.emplace_back(Format(msg));
        }

        return 
            center(
                hcenter({
                    window(text("Chat"),
                        vbox(
                            vbox(mDisplayedMessages, filler()) | flex,
                            hbox(
                                mTypedMessageInput->Render(),
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
    return paragraph(std::format("[{}] {}", message.mUsername, message.mMessage));
}

Component ChatPage::GetPageContent() {
    return mPageContent;
}

