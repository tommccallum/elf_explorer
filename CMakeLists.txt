cmake_minimum_required(VERSION 3.10)
set(PROJ_NAME "elf")
project(${PROJ_NAME} LANGUAGES CXX VERSION 0.1)
string(TIMESTAMP TODAY "%Y-%m-%d %H:%m")

add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

add_subdirectory("src")
add_subdirectory("hellolib")
add_subdirectory("helloworld")