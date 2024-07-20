cmake_minimum_required(VERSION 3.19...4.0.0)

project(STL_learning VERSION 0.1.0 DESCRIPTION "Learning and remake STL of C++!" HOMEPAGE_URL "https://github.com/lhy112233/STL_learning" LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
message(STATUS "C++ standard is ${CMAKE_CXX_STANDARD}")

# GoogleTest option
option(ENABLE_GOOGLETEST "Build and enable GoogleTest" OFF)
# Qt6 option
option(ENABLE_QT6 "Find and config Qt6" OFF)
# Format option
option(ENABLE_FORMAT "Build and enable format" OFF)

add_subdirectory(${CMAKE_SOURCE_DIR}/src)
add_subdirectory(${CMAKE_SOURCE_DIR}/test/array_test)
add_subdirectory(${CMAKE_SOURCE_DIR}/test/memory_test)

# GoogleTest
if(${ENABLE_GOOGLETEST})
    message(STATUS "Enable GoogleTest!")
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG 703bd9caab50b139428cea1aaff9974ebee5742e # release-1.10.0
    )

    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    find_package(GTest REQUIRED)
else()
    message(STATUS "Disable GoogleTest!")
endif()

# Qt6
if(${ENABLE_QT6})
    message(STATUS "enable find and config Qt6")
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTOUIC ON)
    set(CMAKE_AUTORCC ON)
    # find_package()
else()
message(STATUS "enable find and config Qt6")
endif()
