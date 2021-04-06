# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/../googlebench-src
                 ${CMAKE_CURRENT_BINARY_DIR}/../googlebench-build
                 EXCLUDE_FROM_ALL)

add_subdirectory(graph)
