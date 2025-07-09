add_library(shared STATIC 
    src/Socket.cpp
)

target_link_libraries(shared PRIVATE
    ws2_32
)

target_include_directories(shared PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)