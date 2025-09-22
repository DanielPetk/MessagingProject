add_executable(client
    client.cpp
    MainInterface.cpp
    ClientNetworkController.cpp
    ConnectPage.cpp
    ConnectErrorPage.cpp
    ChatPage.cpp
    ClientApp.cpp
)

target_link_libraries(client PRIVATE 
    shared
    ws2_32
    ftxui::component
)

target_include_directories(client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})