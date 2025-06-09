project(playground CXX)

file(GLOB_RECURSE PLAYGROUND_SOURCES CONFIGURE_DEPENDS "**/*.cpp")
file(GLOB_RECURSE PLAYGROUND_HEADERS CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")

add_executable(playground src/main.cpp ${PLAYGROUND_SOURCES})

target_link_libraries(playground PRIVATE libjson)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../json-acceptance-tests/json-checker/
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/data/json-checker)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../data/
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/data/)
