#include "LogPage.h"
#include "MainInterface.h"

using namespace ftxui;

LogPage::LogPage(MainInterface* mainInterface) : Page{mainInterface} {
    mCommandInput = Input(&mCommandInputContent);
    mCommandInput |= CatchEvent([&](Event event) {
         if (event.is_character()) {
            if (mCommandInputContent.size() >= LogPage::MAX_COMMAND_SIZE) { return true; } // Some arbitrary size
        }
        if (event == Event::Return) {
            EnterCommandHelper();
            return true;
        }

        return false;
    });

    mPageContent = Renderer(mCommandInput, [&] {
        return vbox({
            vbox({
                GetServerHeader(),
                mCommandInput->Render()
            }) | size(WIDTH, EQUAL, 80),
        }); 

    });
}

void LogPage::EnterCommandHelper() {
    if (mCommandInputContent.size()) {
        mMainInterface->OnCommandEnter(mCommandInputContent);
    }
}

Element LogPage::GetServerHeader() {
    return vbox({
        text(""),
        text("**************************************"),
        text("*        TERMINAL CHAT SERVER        *"),
        text("**************************************"),
        text(""),
        text("Type 'help' for a list of commads"),
        text("")
    });
}

Component LogPage::GetPageContent() {
    return mPageContent;
}

void LogPage::ClearedTypedCommandField() {
    mCommandInputContent.clear();
}