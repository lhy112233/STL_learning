enable_testing()

add_executable(array_test array_test.cc)

target_link_libraries(array_test PRIVATE  GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(array_test)