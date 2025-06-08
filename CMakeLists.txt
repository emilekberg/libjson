cmake_minimum_required(VERSION 3.31.6)
project(json)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(lib)
add_subdirectory(playground)
add_subdirectory(benchmark)

enable_testing()
add_subdirectory(test)
