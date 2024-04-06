


add_library(MEMORY_LIB INTERFACE)

# target_sources(MEMORY_LIB Unique_ptr.hpp PUBLIC)
target_include_directories(MEMORY_LIB PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})