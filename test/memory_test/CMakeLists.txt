add_executable(unique_ptr_test unique_ptr_test.cc)
target_link_libraries(unique_ptr_test PRIVATE hy_STL)

add_executable(allocator_test allocator_test.cc)
target_link_libraries(allocator_test PRIVATE hy_STL)

add_definitions(-DVERSION=66)