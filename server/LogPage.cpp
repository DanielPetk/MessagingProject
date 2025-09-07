#include "LogPage.h"
#include "MainInterface.h"

using namespace ftxui;

LogPage::LogPage(MainInterface* mainInterface) : Page{mainInterface} {
    
    mDisplayedLogs.reserve(LogPage::MAX_LOG_AMOUNT);

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
        
        mDisplayedLogs.clear();
        for (auto &msg : mLogEntries) {
            mDisplayedLogs.emplace_back(Format(msg));
        }

        Element maybefiller = mDisplayedLogs.size() ? filler() | size(HEIGHT, EQUAL, 1) : filler() | size(HEIGHT, EQUAL, 0);

        return vbox({
            vbox({
                frame(
                    vbox({
                        GetServerHeader(),
                        vbox(mDisplayedLogs),
                        maybefiller,
                        focus(mCommandInput->Render()) | size(WIDTH, EQUAL, 60)
                    })
                ),
            })
        }); 

    });    
}

Element LogPage::Format(const LogEntry& entry) {

    if (entry.mType == LogType::UserCommand) {
        Element maybefiller = mDisplayedLogs.size() ? filler() | size(HEIGHT, EQUAL, 1) : filler() | size(HEIGHT, EQUAL, 0);

        return vbox({
            maybefiller,
            paragraph(entry.mMessage),
            text("")
        });
    }

    std::string type = "Unknown";
    Color logtypecolor = Color::Black;
    if (entry.mType == LogType::Info) {
        type = "INFO";
        logtypecolor = Color::Blue1;
    } else if (entry.mType == LogType::Alert) {
        type = "ALERT";
        logtypecolor = Color::Green3;
    } else if (entry.mType == LogType::Error) {
        type = "ERROR";
        logtypecolor = Color::Red1;
    }

    return hbox({
        hbox({
            filler() | size(WIDTH, EQUAL, 2),
            center({
                hcenter({
                    vcenter({ 
                        text(type) | color(Color::White)
                    })
                })
            }) | flex | bgcolor(logtypecolor),
            filler() | size(WIDTH, EQUAL, 2)
        }) | size(WIDTH, EQUAL, 20),
        paragraph(entry.mMessage)
    });
}

void LogPage::AddLogToList(const LogEntry& entry) {
    if (mLogEntries.size() >= LogPage::MAX_LOG_AMOUNT) {
        mLogEntries.pop_front();
    }
    mLogEntries.emplace_back(entry);
}

void LogPage::ClearLogList() {
    mLogEntries.clear();
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
        text("*                                    *"),
        text("*        TERMINAL CHAT SERVER        *"),
        text("*                                    *"),
        text("**************************************"),
        text(""),
        text("Type 'help' for a list of commands."),
        text("")
    });
}

Component LogPage::GetPageContent() {
    return mPageContent;
}

void LogPage::ClearedTypedCommandField() {
    mCommandInputContent.clear();
}