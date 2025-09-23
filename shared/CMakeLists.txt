add_library(shared STATIC 
    src/Socket.cpp
    src/ProtocolHandler.cpp
)

if(WIN32)
    target_link_libraries(shared PRIVATE ws2_32)
endif()

target_include_directories(shared PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)