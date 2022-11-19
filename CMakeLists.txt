cmake_minimum_required(VERSION 3.23)
project(Sudoku)

set(CMAKE_CXX_STANDARD 20)

add_executable(Sudoku main.cpp)

find_path(RAPIDXML_INCLUDE_DIRS "rapidxml/rapidxml.hpp")
target_include_directories(Sudoku PRIVATE ${RAPIDXML_INCLUDE_DIRS})

find_package(toml11 CONFIG REQUIRED)
target_link_libraries(Sudoku PRIVATE toml11::toml11)