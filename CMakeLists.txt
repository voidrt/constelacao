cmake_minimum_required(VERSION 3.14)
project(Constellations CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
        raylib
        DOWNLOAD_EXTRACT_TIMESTAMP OFF
        URL https://github.com/raysan5/raylib/archive/refs/tags/6.0.tar.gz
)

set(SOURCES
        main.cpp
        src/constants.hpp
        src/utils.hpp
        src/grid/grid.hpp
        src/stars/star.hpp
        src/stars/star.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE include)

target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
