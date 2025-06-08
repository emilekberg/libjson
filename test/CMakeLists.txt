project(libjson_test CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

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


file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../json-acceptance-tests/json-checker/
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/data/json-checker)
