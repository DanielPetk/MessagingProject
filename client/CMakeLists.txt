add_executable(client
    client.cpp
    MainUI.cpp
    ConnectPage.cpp
    ConnectErrorPage.cpp
    ChatPage.cpp
)

target_link_libraries(client PRIVATE 
    shared
    ws2_32
    ftxui::screen
    ftxui::dom
    ftxui::component
)
target_include_directories(client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})