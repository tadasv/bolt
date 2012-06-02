cmake_minimum_required (VERSION 2.8)
project (bolt)

include_directories("${PROJECT_BINARY_DIR}"
                    "${PROJECT_BINARY_DIR}/src"
                    "${PROJECT_BINARY_DIR}/deps/http-parser")

aux_source_directory("${PROJECT_BINARY_DIR}/src" BOLT_SOURCES)

add_library(http_parser STATIC "${PROJECT_BINARY_DIR}/deps/http-parser/http_parser.c")

add_executable(bolt ${BOLT_SOURCES})

target_link_libraries(bolt http_parser ev)
