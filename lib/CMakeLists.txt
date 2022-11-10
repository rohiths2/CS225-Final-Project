# Path definitions.
set(lib_dir ${CMAKE_CURRENT_SOURCE_DIR})

# Add overall libs library.
add_library(libs INTERFACE)
target_include_directories(libs INTERFACE ${lib_dir})
target_link_libraries(libs INTERFACE cs225)