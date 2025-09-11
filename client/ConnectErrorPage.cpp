#include "ConnectErrorPage.h"

#include "MainInterface.h"

using namespace ftxui;

ConnectErrorPage::ConnectErrorPage(MainInterface* mainInterface, const std::string& errorMessage) : Page (mainInterface) {

    mOkayButton = Button("OK", [&] {
        mMainInterface->SetAppState(0);        
    });

    mPageContent = Renderer(mOkayButton, [&, errorMessage] {
        return
            center(
                hcenter({
                    window(text("Error"), 
                        hbox({
                            filler() | size(WIDTH, EQUAL, 1),
                            vbox({
                                separatorEmpty(),
                                paragraph(errorMessage) | center,
                                separatorEmpty(),
                                hbox({filler(), mOkayButton->Render(), filler()})
                            }) | size(WIDTH, EQUAL, 40),
                            filler() | size(WIDTH, EQUAL, 1)
                        })
                    )
                })
            );
        
    });
}

Component ConnectErrorPage::GetPageContent() {
    return mPageContent;
}