add_library(hy_STL INTERFACE)

file(GLOB_RECURSE HEAD_FILES "../include/*.h" "../include/*.hpp" "./*.h" "./*.hpp")
message(STATUS "头文件 : ${HEAD_FILES}")

target_include_directories(hy_STL INTERFACE 
        ${CMAKE_SOURCE_DIR}/include/Array
        ${CMAKE_SOURCE_DIR}/include/Future
        ${CMAKE_SOURCE_DIR}/include/Memory
        ${CMAKE_SOURCE_DIR}/include/Type_traits
        ${CMAKE_SOURCE_DIR}/include/Utility
        ${CMAKE_SOURCE_DIR}/include/Functional
        ${CMAKE_SOURCE_DIR}/include/Tuple)