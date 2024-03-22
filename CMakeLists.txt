cmake_minimum_required(VERSION 3.12)

project(typing-game)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB SOURCES src/*.cpp tui_engine/*.cpp)

add_executable(typing-game ${SOURCES})
