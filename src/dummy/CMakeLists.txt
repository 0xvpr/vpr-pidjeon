cmake_minimum_required(VERSION 3.10.0)

project(dummy C)

set( CMAKE_C_STANDARD          99 )
set( CMAKE_C_STANDARD_REQUIRED ON )

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin )

set( SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src )

set(
    SOURCES
    ${SOURCE_DIR}/dummy.c
)

add_executable(
    ${PROJECT_NAME}
    ${SOURCES}
)

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
    OUTPUT_NAME ${PROJECT_NAME}-${PROJECT_ARCHITECTURE}
    SUFFIX ".exe"
)
