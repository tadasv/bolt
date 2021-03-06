cmake_minimum_required (VERSION 2.8)
project (bolt)

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

#
# Include search paths
#
include_directories("${PROJECT_BINARY_DIR}"
                    "${PROJECT_BINARY_DIR}/src"
                    "${PROJECT_BINARY_DIR}/deps/http-parser")

#
# Main sources
#
aux_source_directory("${PROJECT_BINARY_DIR}/src" BOLT_SOURCES)
aux_source_directory("${PROJECT_BINARY_DIR}/src/db" BOLT_DB_SOURCES)
aux_source_directory("${PROJECT_BINARY_DIR}/src/core" BOLT_CORE_SOURCES)
aux_source_directory("${PROJECT_BINARY_DIR}/src/http" BOLT_HTTP_SOURCES)
aux_source_directory("${PROJECT_BINARY_DIR}/src/handler" BOLT_HANDLER_SOURCES)
# Have all sources but exclude main (bolt.cc)
set(BOLT_SOURCES_NO_MAIN ${BOLT_SOURCES})
list(REMOVE_ITEM BOLT_SOURCES_NO_MAIN "${PROJECT_BINARY_DIR}/src/bolt.cc")

#
# Dependencies
#
add_library(http_parser STATIC "${PROJECT_BINARY_DIR}/deps/http-parser/http_parser.c")

#
# Bolt binary
#
add_executable(bolt "${PROJECT_BINARY_DIR}/src/bolt.cc")
# Make libbolt_core which contains all bolt code except for main() so we could
# link it with unit tests.
add_library(bolt_core STATIC ${BOLT_SOURCES_NO_MAIN} ${BOLT_CORE_SOURCES})
add_library(bolt_db STATIC ${BOLT_DB_SOURCES})
add_library(bolt_http STATIC ${BOLT_HTTP_SOURCES} ${BOLT_HANDLER_SOURCES})
target_link_libraries(bolt bolt_core bolt_db bolt_http http_parser ev jansson)

#
# Unit tests
#
aux_source_directory("${PROJECT_BINARY_DIR}/tests/unit" UNIT_TEST_SOURCES)
add_executable(bolt_unit_tests "${PROJECT_BINARY_DIR}/tests/tests_base.cc" ${UNIT_TEST_SOURCES})
target_link_libraries(bolt_unit_tests
                      bolt_core
                      bolt_http
                      bolt_db
                      gtest http_parser ev jansson)
enable_testing()
add_test(bolt "${EXECUTABLE_OUTPUT_PATH}/bolt_unit_tests")
