cmake_minimum_required(VERSION 3.16)
project(io-multiplexing)

# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_EXTENSIONS OFF)
#
set(CMAKE_C_STANDARD 11)       # Change to C standard (C11, C99, etc.)
set(CMAKE_C_EXTENSIONS OFF)    # Change to C_EXTENSIONS

#
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(IO_MULTIPLEXING_BUILD_TEST ON)
set(IO_MULTIPLEXING_BUILD_SHARED_LIB OFF)

# ============================================================================
# Platform Detection
# ============================================================================
if(APPLE)
    set(PLATFORM "osx")
    message(STATUS "Building for macOS")
elseif(UNIX)
    set(PLATFORM "linux")
    message(STATUS "Building for Linux")
else()
    message(FATAL_ERROR "Unsupported platform")
endif()

# ============================================================================
# Build Configuration
# ============================================================================
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Building in Debug mode")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wextra")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Building in Release mode")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -DNDEBUG")
endif()

# ============================================================================
# Server Executable
# ============================================================================
file(GLOB SHARED_UTILS_SRC "src/shared/*.c")
file(GLOB SERVER_SRC "src/server/*.c")
file(GLOB CLIENT_SRC "src/client/*.c")
file(GLOB PROJ_SRC "src/*.c")
file(GLOB SERVER_PLATFORM_SRC "src/server/${PLATFORM}/*.c")

if(IO_MULTIPLEXING_BUILD_SHARED_LIB)
    add_library(${PROJECT_NAME} SHARED 
        ${SERVER_SRC}
        ${SERVER_PLATFORM_SRC}
        ${CLIENT_SRC}
        ${SHARED_UTILS_SRC}
        ${PROJ_SRC}
    )
else()
    add_library(${PROJECT_NAME} STATIC
        ${SERVER_SRC}
        ${SERVER_PLATFORM_SRC}
        ${CLIENT_SRC}
        ${SHARED_UTILS_SRC}
        ${PROJ_SRC}
    )
endif()

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# ============================================================================
# Testing (Optional)
# ============================================================================
if(IO_MULTIPLEXING_BUILD_TEST)
    enable_testing()
    add_subdirectory(test)
endif()
