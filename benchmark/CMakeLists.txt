project(libjson_bench CXX)


include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.17.0)

set(gtest_force_shared_crt
    ON
    CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

FetchContent_Declare(
  benchmark
  GIT_REPOSITORY https://github.com/google/benchmark.git
  GIT_TAG main  # or release version like v1.8.4
)
FetchContent_MakeAvailable(benchmark)

file(GLOB_RECURSE LIBJSON_BENCH_SUBDIRS CONFIGURE_DEPENDS "cases/**/*.bench.cpp")
file(GLOB_RECURSE LIBJSON_BENCH_ROOT CONFIGURE_DEPENDS "cases/*.bench.cpp")

add_executable(libjson_bench ${LIBJSON_BENCH_SUBDIRS} ${LIBJSON_BENCH_ROOT})

target_link_libraries(libjson_bench PRIVATE GTest::gtest_main benchmark libjson)

include(GoogleTest)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../data/
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/data/)
