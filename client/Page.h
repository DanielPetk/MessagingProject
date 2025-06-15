#pragma once 
#include <ftxui/component/component.hpp>
class MainUI;

class Page {

protected:
    MainUI* mMainUI;
public:
    Page(MainUI* mainUI) : mMainUI{mainUI} {}
    virtual ftxui::Component GetPageContent() = 0;    
};