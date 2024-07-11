cmake_minimum_required(VERSION 3.19...4.0.0)

project(STL_learning VERSION 0.1.0 DESCRIPTION "Learning and remake STL of C++!" HOMEPAGE_URL "https://github.com/lhy112233/STL_learning" LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
message(STATUS "C++ standard is ${CMAKE_CXX_STANDARD}")

# GoogleTest option
option(ENABLE_GOOGLETEST "Build and enable GoogleTest" OFF)

add_subdirectory(${CMAKE_SOURCE_DIR}/src)
add_subdirectory(${CMAKE_SOURCE_DIR}/test/array_test)
add_subdirectory(${CMAKE_SOURCE_DIR}/test/memory_test)

# GoogleTest
if(${ENABLE_GOOGLETEST})
    message(STATUS "Enable GoogleTest!")
    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
    )

    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

else()
    message(STATUS "Disable GoogleTest!")
endif()