cmake_minimum_required (VERSION 2.8)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/../googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/../googletest-build
                 EXCLUDE_FROM_ALL)

# Now simply link against gtest or gtest_main as needed. Eg
add_executable(graphs main.cpp)
target_link_libraries(graphs gtest_main)
target_compile_features(graphs PUBLIC cxx_std_17)
#add_test(NAME example_test COMMAND example)
