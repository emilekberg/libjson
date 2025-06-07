project(libjson CXX)

file(GLOB_RECURSE LIBJSON_SOURCES CONFIGURE_DEPENDS "src/**/*.cpp")
file(GLOB_RECURSE LIBJSON_HEADERS CONFIGURE_DEPENDS "src/**/*.h")

add_library(libjson)
target_sources(
  libjson
  PUBLIC ${LIBJSON_HEADERS}
  PRIVATE ${LIBJSON_SOURCES})

target_include_directories(libjson PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
