add_executable(server
    server.cpp
    ServerApp.cpp
    ServerNetworkController.cpp
    MainInterface.cpp
    LogPage.cpp
)

target_link_libraries(server PRIVATE
    shared
    ftxui::component
)

if(WIN32)
    target_link_libraries(server PRIVATE ws2_32)
endif()

target_include_directories(server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})