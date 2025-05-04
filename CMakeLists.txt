cmake_minimum_required(VERSION 3.15)
project(MessagingProject)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

FetchContent_Declare(
  ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI
  GIT_TAG v6.0.2
)

FetchContent_MakeAvailable(ftxui)


# Add client and server subdirectories
add_subdirectory(client)
add_subdirectory(server)