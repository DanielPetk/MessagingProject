#include "ConnectErrorPage.h"

#include "MainInterface.h"

using namespace ftxui;

ConnectErrorPage::ConnectErrorPage(MainInterface* mainInterface) : Page (mainInterface) {

    mOkayButton = Button("OK", [&] {
        mMainInterface->SetAppState(0);        
    });

    mPageContent = Renderer(mOkayButton, [&] {
        return window(text("Connection Error"),
            center(
                hcenter(
                    vbox({
                        separatorEmpty(),
                        paragraph("There was an error connecting to the server."),
                        separatorEmpty(),
                        hbox({filler(), mOkayButton->Render(), filler()})
                    })
                )
            )
        );
    });
}

Component ConnectErrorPage::GetPageContent() {
    return mPageContent;
}