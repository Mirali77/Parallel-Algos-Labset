cmake_minimum_required(VERSION 3.10)
project(ParallelAlgos)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Parlay REQUIRED)

include(FetchContent)

FetchContent_Declare(
  parlay
  GIT_REPOSITORY https://github.com/cmuparlay/parlaylib.git
)
FetchContent_MakeAvailable(parlay)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.17.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(parallel_algos
    src/bfs.cpp
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

add_library(bfs_lib
    src/bfs.cpp
    src/utils.cpp
)
target_link_libraries(bfs_lib PUBLIC Parlay::parlay)

add_executable(
    quicksort_tests
    tests/quicksort_ut.cpp
)
target_link_libraries(quicksort_tests PRIVATE quicksort_lib gtest gtest_main)

add_executable(
    bfs_tests
    tests/bfs_ut.cpp
)
target_link_libraries(bfs_tests PRIVATE bfs_lib gtest gtest_main)

enable_testing()
add_test(NAME quicksort_tests COMMAND quicksort_tests)
add_test(NAME bfs_tests COMMAND bfs_tests)