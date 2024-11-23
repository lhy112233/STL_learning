add_executable(io_test spmc_queue_test.cc)

target_link_libraries(io_test PRIVATE hy_STL)
