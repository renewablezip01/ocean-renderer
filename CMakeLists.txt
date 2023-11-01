cmake_minimum_required(VERSION 3.7)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

project(GraphicsProgramming VERSION 1.0 LANGUAGES CXX)
add_subdirectory(dependencies/glad/)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(GraphicsProgramming src/main.cpp)
target_link_libraries(GraphicsProgramming ${SDL2_LIBRARIES} glad)
