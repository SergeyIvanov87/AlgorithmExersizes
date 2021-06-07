# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/../googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/../googletest-build
                 EXCLUDE_FROM_ALL)

add_subdirectory(data_structure)

add_subdirectory(graph)
add_subdirectory(heap)
add_subdirectory(matrix)
add_subdirectory(max_subarray)
add_subdirectory(sort)
