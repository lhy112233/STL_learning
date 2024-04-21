cmake_minimum_required(VERSION 3.19...4.0.0)

project(STL_learning VERSION 0.1.0 DESCRIPTION "" HOMEPAGE_URL "" LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


add_subdirectory(${CMAKE_SOURCE_DIR}/src)
add_subdirectory(${CMAKE_SOURCE_DIR}/test/array_test)
# add_subdirectory(${CMAKE_SOURCE_DIR}/test/memory_test)


