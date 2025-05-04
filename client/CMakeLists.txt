add_executable(client
    client.cpp
    ConnectDialog.cpp 
    ConnectDialog.h
)

target_link_libraries(client PRIVATE 
    ws2_32
    ftxui::screen
    ftxui::dom
    ftxui::component
)
target_include_directories(client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})