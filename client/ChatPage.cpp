#include "ChatPage.h"
#include "MainUI.h"

using namespace ftxui;

ChatPage::ChatPage(MainUI* mainUI) : Page(mainUI) {

};

Component ChatPage::GetPageContent() {
    return Button("Chat Page Test", [&]{});
}

