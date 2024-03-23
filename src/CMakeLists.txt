cmake_minimum_required( VERSION 3.22.0 )

# --------------- vpr-pidjeon --------------- #

if ( "${PROJECT_ARCHITECTURE}" STREQUAL "x64")
    ## Initialize project
    project( vpr-pidjeon
             VERSION        1.0.0
             LANGUAGES      C
    )

    set( CMAKE_C_STANDARD_REQUIRED ON )
    set( CMAKE_C_STANDARD          99 )

    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/vpr-pidjeon/lib )

    set( SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR} )
    set( INCLUDE_DIR ${SOURCE_DIR} )
    set( SOURCES
         # C sources
         ${SOURCE_DIR}/crt.c
         ${SOURCE_DIR}/loadlibrary.c
         ${SOURCE_DIR}/logger.c
         ${SOURCE_DIR}/manualmap.c
         ${SOURCE_DIR}/util.c
    )

    include_directories( ${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR} )

    add_library(
        ${PROJECT_NAME}
        SHARED
        ${SOURCES}
    )

    target_link_libraries( ${PROJECT_NAME} psapi )

    set_target_properties(
        ${PROJECT_NAME}
        PROPERTIES
        OUTPUT_NAME ${PROJECT_NAME}
        PREFIX ""
        SUFFIX ".dll"
    )
endif()
