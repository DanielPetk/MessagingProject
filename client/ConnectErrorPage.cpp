#include "ConnectErrorPage.h"

#include "MainInterface.h"

using namespace ftxui;

ConnectErrorPage::ConnectErrorPage(MainInterface* mainInterface, const std::string& errorMessage) : Page (mainInterface) {

    mOkayButton = Button("OK", [&] {
        mMainInterface->SetAppState(0);        
    });

    mPageContent = Renderer(mOkayButton, [&] {
        return window(text("Connection Error"),
            center(
                hcenter(
                    vbox({
                        separatorEmpty(),
                        paragraph(errorMessage),
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