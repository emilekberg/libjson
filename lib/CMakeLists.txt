project(libjson CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(LIBJSON_OPTIMIZED_BUILD "Always optimize libjson regardless of parent build type" ON)


file(GLOB_RECURSE LIBJSON_SOURCES CONFIGURE_DEPENDS "src/**/*.cpp")
file(GLOB_RECURSE LIBJSON_HEADERS CONFIGURE_DEPENDS "src/**/*.h")

add_library(libjson)

if(LIBJSON_OPTIMIZED_BUILD)
  target_compile_options(libjson PRIVATE -O3)
endif()

target_sources(
  libjson
  PUBLIC ${LIBJSON_HEADERS}
  PRIVATE ${LIBJSON_SOURCES})

target_include_directories(libjson PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
