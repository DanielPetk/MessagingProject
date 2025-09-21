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

# if(MSVC)
#     set_target_properties(client PROPERTIES
#         COMPILE_PDB_NAME "client"
#         COMPILE_PDB_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/client"
#     )
#     add_compile_options(/FS)
# endif()

target_include_directories(client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})