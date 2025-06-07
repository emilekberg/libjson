project(libjson_test CXX)

include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.17.0)

set(gtest_force_shared_crt
    ON
    CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

file(GLOB_RECURSE LIBJSON_TEST_SUBDIRS CONFIGURE_DEPENDS "cases/**/*.test.cpp")
file(GLOB_RECURSE LIBJSON_TEST_ROOT CONFIGURE_DEPENDS "cases/*.test.cpp")

add_executable(libjson_test ${LIBJSON_TEST_SUBDIRS} ${LIBJSON_TEST_ROOT})

target_link_libraries(libjson_test PRIVATE GTest::gtest_main libjson)

include(GoogleTest)

enable_testing()
gtest_discover_tests(libjson_test)
