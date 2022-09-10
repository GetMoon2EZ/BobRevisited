cmake_minimum_required(VERSION 3.9)

# Project name 
set(PROJECT_NAME "bob_revisited")
project(${PROJECT_NAME} VERSION 0.0.1 DESCRIPTION "Bob project from university revisited using cpp")

# CPP standards
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Executable output path
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Include directories
include_directories(
    ./include/
)

add_subdirectory(src)