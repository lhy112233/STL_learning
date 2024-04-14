cmake_minimum_required(VERSION 3.19...4.0.0)
project(STL_learning VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()
add_subdirectory(${CMAKE_SOURCE_DIR}/3rdparty/googletest)
include_directories(${CMAKE_SOURCE_DIR}/3rdparty/googletest/googletest/include)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

# test
add_subdirectory(${CMAKE_SOURCE_DIR}/test/array_test)
