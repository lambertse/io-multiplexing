include_directories(${CMAKE_SOURCE_DIR})

# Create integrate_test 
add_executable(server_integration
   integrate_test/server.c 
   ${TEST_SHARED_SRC}
)
target_link_libraries(server_integration io-multiplexing)
target_include_directories(server_integration PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_executable(client_integration
   integrate_test/client.c 
   ${TEST_SHARED_SRC}
)
target_link_libraries(client_integration io-multiplexing)
target_include_directories(client_integration PRIVATE ${CMAKE_SOURCE_DIR}/include)


##########


# Unit test
add_executable(rb_tree_unit_test 
    unit_test/rb_tree_ut.c
)


##########

# Bench mark

