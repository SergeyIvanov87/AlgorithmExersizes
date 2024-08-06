cmake_minimum_required (VERSION 3.0)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.



file (GLOB SUITE_SRC *.cpp)
foreach (file ${SUITE_SRC})
    get_filename_component(filename ${file} NAME_WLE)
    get_filename_component(dirname ${file} DIRECTORY)

    add_executable(others_${filename} ${filename}.cpp)
    target_link_libraries(others_${filename}  gtest_main)
    target_include_directories(others_${filename}  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../..>)

    target_compile_features(others_${filename}  PUBLIC cxx_std_17)
endforeach (file ${SUITE_SRC})
