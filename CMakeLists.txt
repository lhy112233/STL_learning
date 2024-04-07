cmake_minimum_required(VERSION 3.19...4.0.0)
project(STL_learning VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

# add_subdirectory(${CMAKE_SOURCE_DIR}/future)
# add_subdirectory(${CMAKE_SOURCE_DIR}/utility)
# add_subdirectory(${CMAKE_SOURCE_DIR}/memory)
# add_subdirectory(${CMAKE_SOURCE_DIR}/array/Array.hpp)