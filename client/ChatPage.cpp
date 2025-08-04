#include "ChatPage.h"

#include "ClientApp.h"

using namespace ftxui;

ChatPage::ChatPage(ClientApp* clientApp) : Page(clientApp) {

};

Component ChatPage::GetPageContent() {
    return Button("Chat Page Test", [&]{});
}

