#pragma once 
#include <ftxui/component/component.hpp>
class ClientApp;

class Page {

protected:
    ClientApp* mClientApp;
public:
    Page(ClientApp* clientApp) : mClientApp{clientApp} {}
    virtual ftxui::Component GetPageContent() = 0;    
};