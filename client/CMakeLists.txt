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
    ftxui::component
)

if(WIN32)
    target_link_libraries(client PRIVATE ws2_32)
endif()


target_include_directories(client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})