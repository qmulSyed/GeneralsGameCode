project(cnc-zero-hour C CXX)

cmake_minimum_required(VERSION 3.23)

if(WIN32)
    add_subdirectory(extern/d3d8)
    add_subdirectory(extern/GamespySDK)
    add_subdirectory(extern/gnu_regex)
endif()

add_subdirectory(Libraries)
add_subdirectory(GameEngine)
