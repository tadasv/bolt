cmake_minimum_required (VERSION 2.8)
project (bolt)

include_directories("${PROJECT_BINARY_DIR}"
                    "${PROJECT_BINARY_DIR}/src"
                    "${PROJECT_BINARY_DIR}/deps/http-parser")

set(CMAKE_C_FLAGS "-Wall -Werror")
set(CMAKE_CXX_FLAGS "-Wall -Werror")

if (CMAKE_BUILD_TYPE MATCHES Release)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
endif (CMAKE_BUILD_TYPE MATCHES Release)

if (CMAKE_BUILD_TYPE MATCHES Debug)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
   set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
   set(CMAKE_CXX_DEFINES "${CMAKE_CXX_DEFINES} -DDEBUG")
   set(CMAKE_C_DEFINES "${CMAKE_CXX_DEFINES} -DDEBUG")
endif (CMAKE_BUILD_TYPE MATCHES Debug)


set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/build")
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/build")

aux_source_directory("${PROJECT_BINARY_DIR}/src" BOLT_SOURCES)
aux_source_directory("${PROJECT_BINARY_DIR}/tests/unit" UNIT_TEST_SOURCES)

add_library(http_parser STATIC "${PROJECT_BINARY_DIR}/deps/http-parser/http_parser.c")

add_executable(bolt_unit_tests ${UNIT_TEST_SOURCES} "${PROJECT_BINARY_DIR}/src/http_request.cc")
add_executable(bolt ${BOLT_SOURCES})

target_link_libraries(bolt http_parser ev)
target_link_libraries(bolt_unit_tests gtest gtest_main http_parser)

enable_testing()
add_test(bolt "${EXECUTABLE_OUTPUT_PATH}/bolt_unit_tests")
