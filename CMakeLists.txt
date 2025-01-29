cmake_minimum_required(VERSION 3.30)
project(task)

set(CMAKE_CXX_STANDARD 20)

add_executable(
        task
        src/main.cpp
        src/utilities.cpp
        src/Event.cpp
        src/ComputerClub.cpp
        src/Table.cpp
)

target_include_directories(task PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)