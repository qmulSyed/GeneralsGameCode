project(cnc-zero-hour C CXX)

cmake_minimum_required(VERSION 3.23)

file(GLOB_RECURSE SOURCES_COMMON
    ${CMAKE_CURRENT_SOURCE_DIR}/GameEngine/Source/Common/*.cpp
)

# Remove Debug.cpp from the common sources for UNIX
if(UNIX)
    list(REMOVE_ITEM SOURCES_COMMON ${CMAKE_CURRENT_SOURCE_DIR}/GameEngine/Source/Common/System/Debug.cpp)
    list(REMOVE_ITEM SOURCES_COMMON ${CMAKE_CURRENT_SOURCE_DIR}/GameEngine/Source/Common/System/registry.cpp)
else()
    list(REMOVE_ITEM SOURCES_COMMON ${CMAKE_CURRENT_SOURCE_DIR}/GameEngine/Source/Common/System/DebugUnix.cpp)
    list(REMOVE_ITEM SOURCES_COMMON ${CMAKE_CURRENT_SOURCE_DIR}/GameEngine/Source/Common/System/RegistryUnix.cpp)
endif()

add_library(cnc-zero-hour-common STATIC ${SOURCES_COMMON})

target_include_directories(cnc-zero-hour-common PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/GameEngine/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/GameEngine/Include/Precompiled
    # Used in Thing.h via WWMath
    ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/Source/WWVegas
    # Also pulls WWLib
    ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/Source/WWVegas/WWLib
    # Pulled by wwstring.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/Source/WWVegas/WWDebug
    # Required by DataChunk.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/Source/Compression
)

if(WIN32)
    add_subdirectory(extern/d3d8)
    target_link_libraries(cnc-zero-hour-common PRIVATE d3d8)
else()
    target_include_directories(cnc-zero-hour-common PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/extern/dxvk-native/usr/include/dxvk
        ${CMAKE_CURRENT_SOURCE_DIR}/extern/compat
    )
endif()

if(CMAKE_GENERATOR MATCHES "Visual Studio")    
else()
    target_compile_options(cnc-zero-hour-common PRIVATE -Wno-invalid-offsetof)
endif()