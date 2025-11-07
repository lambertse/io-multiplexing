include_directories(${CMAKE_SOURCE_DIR})

# Protobuf 
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.14.0
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Configure googletest options
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

# file(GLOB TEST_SHARED_SRC "${CMAKE_CURRENT_SOURCE_DIR}/shared/*.cpp")

# Create test executable
add_executable(server_integration
   integrate_test/server.cpp 
   ${TEST_SHARED_SRC}
)
target_link_libraries(server_integration io-multiplexing_cpp)
target_include_directories(server_integration PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_executable(client_integration
   integrate_test/client.cpp 
   ${TEST_SHARED_SRC}
)
target_link_libraries(client_integration io-multiplexing_cpp)
target_include_directories(client_integration PRIVATE ${CMAKE_SOURCE_DIR}/include)
