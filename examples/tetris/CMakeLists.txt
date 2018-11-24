cmake_minimum_required(VERSION 3.7)

project(tetris)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

INCLUDE(FindPkgConfig)

PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)

file(GLOB_RECURSE PROJECT_SOURCES
    "src/*.h"
    "src/*.hpp"
    "src/*.cpp"

    "../../src/*.h"
    "../../src/*.hpp"
    "../../src/*.cpp"
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

include_directories(
    ${SDL2_INCLUDE_DIRS}
    ${SDL2IMAGE_INCLUDE_DIRS}
    "../../src/"
)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES})
