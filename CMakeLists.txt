cmake_minimum_required(VERSION 3.7)

project(GraphicsProgramming VERSION 1.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(dependencies/glad/)
add_subdirectory(dependencies/SDL/)

include_directories(dependencies/SDL/include/)

add_executable(GraphicsProgramming src/main.cpp)
target_link_libraries(GraphicsProgramming SDL2-static glad)
