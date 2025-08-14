#include "ChatPage.h"

#include "MainInterface.h"

using namespace ftxui;

ChatPage::ChatPage(MainInterface* mainInterface) : Page(mainInterface) {

};

Component ChatPage::GetPageContent() {
    return Button("Chat Page Test", [&]{});
}

