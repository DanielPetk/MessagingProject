#include "LogPage.h"
#include "MainInterface.h"

using namespace ftxui;

LogPage::LogPage(MainInterface* mainInterface) : Page{mainInterface} {

}

Component LogPage::GetPageContent() {
    return Input("Test");
}

