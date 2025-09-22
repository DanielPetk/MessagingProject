add_executable(server
    server.cpp
    ServerApp.cpp
    ServerNetworkController.cpp
    MainInterface.cpp
    LogPage.cpp
)

target_link_libraries(server PRIVATE
    shared
    ws2_32
    ftxui::component
)

target_include_directories(server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})