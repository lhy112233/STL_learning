cmake_minimum_required(VERSION 3.19...4.0.0)

project(STL_learning VERSION 0.1.0 DESCRIPTION "" HOMEPAGE_URL "" LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD ON)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/source-charset:utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/execution-charset:gbk>")
endif()

add_subdirectory(${CMAKE_SOURCE_DIR}/src)
# add_subdirectory(${CMAKE_SOURCE_DIR}/3rdparty/googletest)
add_subdirectory(${CMAKE_SOURCE_DIR}/test)


