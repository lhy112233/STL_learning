add_library(UTILITY_LIB INTERFACE)

# target_sources(UTILITY_LIB Utility.hpp PUBLIC)
target_include_directories(UTILITY_LIB PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})