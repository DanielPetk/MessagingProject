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

# if(MSVC)
#     set_target_properties(server PROPERTIES
#         COMPILE_PDB_NAME "server"
#         COMPILE_PDB_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/server"
#     )
#     add_compile_options(/FS)
# endif()

target_include_directories(server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})