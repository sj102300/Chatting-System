cmake_minimum_required(VERSION 3.10)

# Project name
project(SocketProgramming)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Debug mode 설정
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")
endif()

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/server
    ${CMAKE_CURRENT_SOURCE_DIR}/client
)

# Configure file for paths
include_directories(${CMAKE_BINARY_DIR})

# Main application source files (excluding unnecessary files)
set(APP_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/server/server.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/server/client_thread.cc
)

# Main application header files
set(APP_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/server/shared.h
    ${CMAKE_CURRENT_SOURCE_DIR}/server/client_thread.h
)

# Add executable
add_executable(${PROJECT_NAME} 
    ${APP_SOURCES} 
    ${APP_HEADERS}
)
