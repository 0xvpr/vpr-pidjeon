cmake_minimum_required(VERSION 3.10.0)

project(payload C)

set( CMAKE_C_STANDARD          99 )
set( CMAKE_C_STANDARD_REQUIRED ON )

set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib )

set( SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src )

set(
    SOURCES
    ${SOURCE_DIR}/payload.c
)

add_library(
    ${PROJECT_NAME}
    SHARED
    ${SOURCES}
)

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
    OUTPUT_NAME ${PROJECT_NAME}-${PROJECT_ARCHITECTURE}
    PREFIX ""
    SUFFIX ".dll"
)
