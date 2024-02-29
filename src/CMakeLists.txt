cmake_minimum_required( VERSION 3.10.0 )

# --------------- vpr-pidjeon --------------- #

## Initialize project
project( vpr-pidjeon C )

## Set output directory
#set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib )

## Source directories
set( SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR} )

## Include directories
include_directories( ${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR} )
set( INCLUDE_DIR ${SOURCE_DIR} )

## Add C sources
set(
    SOURCES
    # C sources
    ${SOURCE_DIR}/crt.c
    ${SOURCE_DIR}/injector.c
    ${SOURCE_DIR}/loadlibrary.c
    ${SOURCE_DIR}/logger.c
    ${SOURCE_DIR}/manualmap.c
    ${SOURCE_DIR}/mem.c
    ${SOURCE_DIR}/parser.c
    ${SOURCE_DIR}/util.c
    # C includes
    ${INCLUDE_DIR}/crt.h
    ${INCLUDE_DIR}/definitions.h
    ${INCLUDE_DIR}/injector.h
    ${INCLUDE_DIR}/loadlibrary.h
    ${INCLUDE_DIR}/logger.h
    ${INCLUDE_DIR}/manualmap.h
    ${INCLUDE_DIR}/mem.h
    ${INCLUDE_DIR}/parser.h
    ${INCLUDE_DIR}/util.h
)

## Application executable and link dependencies
add_library( ${PROJECT_NAME} SHARED ${SOURCES} )

## Link dependencies
target_link_libraries( ${PROJECT_NAME} psapi )

## Set architecture suffix
set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
    OUTPUT_NAME ${PROJECT_NAME}
    PREFIX ""
    SUFFIX ".dll"
)

## ------------- Wrapper ------------- #
#if ( ${PROJECT_ARCHITECTURE} STREQUAL "x64" )
#    project( vpr-pidjeon-wrapper C )
#
#    ## Set output directory
#    set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin )
#
#    ## Include directories
#    include_directories( ${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/include )
#    set( INCLUDE_DIR include )
#
#    ## Source directories
#    set( SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src )
#
#    ## Add C sources
#    set(
#        SOURCES
#        # C sources
#        ${SOURCE_DIR}/mem.c
#        ${SOURCE_DIR}/util.c
#        ${SOURCE_DIR}/wrapper.c
#        # C includes
#        ${INCLUDE_DIR}/mem.h
#        ${INCLUDE_DIR}/pe32.h
#        ${INCLUDE_DIR}/util.h
#    )
#
#    ## Application executable and link dependencies
#    add_executable( ${PROJECT_NAME} ${SOURCES} )
#
#    ## Link Dependencies
#    target_link_libraries( ${PROJECT_NAME} psapi )
#
#    ## Set architecture suffix
#    string(REPLACE "-wrapper" "" BINARY_NAME ${PROJECT_NAME} )
#    set_target_properties(
#        ${PROJECT_NAME}
#        PROPERTIES
#        OUTPUT_NAME ${BINARY_NAME}
#        SUFFIX ".exe"
#    )
#endif()
