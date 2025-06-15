#include "ConnectErrorPage.h"
#include "MainUI.h"

using namespace ftxui;

ConnectErrorPage::ConnectErrorPage(MainUI* mainUI) : Page (mainUI) {

    mOkayButton = Button("OK", [&] {
        mMainUI->SetAppState(0);        
    });

    mPageContent = Renderer(mOkayButton, [&] {
        return window(text("Connection Error"),
        hbox({
            separatorEmpty(),
            vbox({
                separatorEmpty(),
                paragraph("There was an error connecting to the server."),
                separatorEmpty(),
                hbox({filler(), mOkayButton->Render(), filler()})
            }), 
            separatorEmpty()
            })
        );
    });
}

Component ConnectErrorPage::GetPageContent() {
    return mPageContent;
}