cmake_minimum_required(VERSION 3.16)
project(QtHttp VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

option(BUILD_SERVER "Build HTTP server application" ON)
option(BUILD_CLIENT "Build HTTP client application" ON)
option(BUILD_TESTS "Build tests" OFF)

find_package(Qt6 REQUIRED COMPONENTS Core Network)

add_subdirectory(lib)

if(BUILD_SERVER)
    add_subdirectory(server)
endif()

if(BUILD_CLIENT)
    add_subdirectory(client)
endif()
