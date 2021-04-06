cmake_minimum_required (VERSION 3.0)

project(Exersizes
        VERSION ""
        LANGUAGES CXX)


#### GTEST ####
# See `https://github.com/google/googletest/blob/master/googletest/README.md`
#Download and unpack googletest at configure time
configure_file(cmake/ExternalGTest.cmake.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()

execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

#### GBench ####
configure_file(cmake/ExternalGBench.cmake.in googlebench-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googlebench-download )
if(result)
  message(FATAL_ERROR "CMake step for googlebench failed: ${result}")
endif()

execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googlebench-download )
if(result)
  message(FATAL_ERROR "Build step for googlebench failed: ${result}")
endif()



# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)



# Exersizes
add_subdirectory(graphs)

# Test
add_subdirectory(test)

# Benchmark
add_subdirectory(bench)
