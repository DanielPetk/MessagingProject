add_executable(server
    server.cpp
)

target_link_libraries(server PRIVATE
    shared
    ws2_32
)
target_include_directories(server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})