cmake_minimum_required(VERSION 3.10)
project(ParallelAlgos)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Parlay REQUIRED)

add_executable(parallel_algos
    src/main.cpp
    src/quicksort.cpp
    src/utils.cpp
)

target_link_libraries(parallel_algos PRIVATE Parlay::parlay)

add_library(quicksort_lib
    src/quicksort.cpp
    src/utils.cpp
)
target_link_libraries(quicksort_lib PUBLIC Parlay::parlay)

include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.17.0
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(
    parallel_algos_tests
    tests/quicksort_ut.cpp
)
target_link_libraries(parallel_algos_tests PRIVATE quicksort_lib gtest gtest_main)

enable_testing()
add_test(NAME parallel_algos_tests COMMAND parallel_algos_tests)
