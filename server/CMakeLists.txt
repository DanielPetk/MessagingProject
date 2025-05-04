add_executable(server
    server.cpp
)

target_link_libraries(server PRIVATE
    ws2_32
)
target_include_directories(client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})