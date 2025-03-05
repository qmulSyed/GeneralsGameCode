project(cnc-zero-hour C CXX)

cmake_minimum_required(VERSION 3.23)

if(WIN32)
    add_subdirectory(extern/d3d8)
    add_subdirectory(extern/GamespySDK)
endif()

add_subdirectory(Libraries)
add_subdirectory(GameEngine)
add_subdirectory(GameEngineDevice)

if(WIN32)
    add_executable(RTS WIN32 Main/WinMain.cpp)
    target_link_libraries(RTS GameEngine GameEngineDevice)
endif()