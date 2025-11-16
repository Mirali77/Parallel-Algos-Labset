cmake_minimum_required(VERSION 3.10)
project(ParallelAlgos)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Parlay REQUIRED)

add_executable(parallel_algos
    src/main.cpp
    src/quicksort.cpp
)

target_link_libraries(parallel_algos PRIVATE Parlay::parlay)
