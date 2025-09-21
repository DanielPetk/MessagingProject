add_library(shared STATIC 
    src/Socket.cpp
    src/ProtocolHandler.cpp
)

target_link_libraries(shared PRIVATE
    ws2_32
)

# if(MSVC)
#     set_target_properties(shared PROPERTIES
#         COMPILE_PDB_NAME "shared"
#         COMPILE_PDB_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/shared"
#     )
#     add_compile_options(/FS)
# endif()

target_include_directories(shared PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)