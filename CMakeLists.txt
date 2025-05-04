cmake_minimum_required(VERSION 3.15)
project(MessagingProject)

set(CMAKE_CXX_STANDARD 17)




# Add client and server subdirectories
add_subdirectory(client)
add_subdirectory(server)