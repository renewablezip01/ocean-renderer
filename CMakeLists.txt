cmake_minimum_required(VERSION 3.7)

project(GraphicsProgramming VERSION 1.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Set C++ version
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(glm CONFIG REQUIRED)

add_subdirectory(dependencies/glad/)
add_subdirectory(dependencies/SDL/)

include_directories(dependencies/SDL/include/)
include_directories(src/) # For easier includes

add_executable(GraphicsProgramming 
  src/main.cpp src/core/stb_image.h 
  src/core/localFile.cpp src/core/localFile.hpp 
  src/shaders/shader.cpp src/shaders/shader.hpp 
  src/core/camera/camera.hpp src/core/camera/camera.cpp
)

target_link_libraries(GraphicsProgramming SDL2-static glad glm::glm)
target_precompile_headers(GraphicsProgramming PRIVATE src/precompiled.hpp)
