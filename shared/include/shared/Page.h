#pragma once 

#include <ftxui/component/component.hpp>

class MainInterface;

class Page {

protected:
    MainInterface* mMainInterface;
public:
    Page(MainInterface* mainInterface) : mMainInterface{mainInterface} {}
    virtual ftxui::Component GetPageContent() = 0;    
};