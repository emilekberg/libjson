cmake_minimum_required(VERSION 3.31.6)
project(json)
set(CMAKE_CXX STANDARD 23)
set(CMAKE_EXPORT_COMPILE_OPTIONS ON)

add_subdirectory(lib)
add_subdirectory(playground)

enable_testing()
add_subdirectory(test)
