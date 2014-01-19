cmake_minimum_required(VERSION 2.8)
project("FastCGI server")

# Path to additional CMake modules
set(CMAKE_MODULE_PATH
    ${CMAKE_SOURCE_DIR}/CMake
    ${CMAKE_MODULE_PATH})

find_package(FCGI REQUIRED)

add_subdirectory(src)
