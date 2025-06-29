cmake_minimum_required(VERSION 3.31.6)
project(json)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


add_subdirectory(lib)


if(BUILD_TESTING)
	add_subdirectory(benchmark)
endif()
include(CTest) 
if(BUILD_TESTING)
	add_subdirectory(playground)
	enable_testing()
	add_subdirectory(test)
endif()
