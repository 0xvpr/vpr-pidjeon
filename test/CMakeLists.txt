cmake_minimum_required( VERSION 3.22.0 )

# --------------------------------------------------------------------------- #
#                               Dummy Target                                  #
# --------------------------------------------------------------------------- #

project(dummy C)

set( CMAKE_C_STANDARD_REQUIRED ON )
set( CMAKE_C_STANDARD          99 )

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin )

set( SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR} )

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

# --------------------------------------------------------------------------- #
#                              Payload Target                                 #
# --------------------------------------------------------------------------- #

project(payload C)

set( CMAKE_C_STANDARD          99 )
set( CMAKE_C_STANDARD_REQUIRED ON )

set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib )

set( SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR} )

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

# --------------------------------------------------------------------------- #
#                                Live Tests                                   #
# --------------------------------------------------------------------------- #


if ( "${PROJECT_ARCHITECTURE}" STREQUAL "x64")
    ## Add testing support
    project( run-tests C )

    ## Set output directory
    set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin )

    ## Source directories
    set( SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
    set( TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR} )

    ## Include directories
    include_directories( ${PROJECT_NAME} ${SOURCE_DIR} )
    include_directories( ${PROJECT_NAME} /usr/local/share/mingw64/include )
    set( INCLUDE_DIR ${SOURCE_DIR} )


    ## Add CXX sources
    set(
        TEST_SOURCES
        # C sources
        ${TEST_DIR}/unit_test_pidjeon.c
        ${SOURCE_DIR}/crt.c
        ${SOURCE_DIR}/loadlibrary.c
        ${SOURCE_DIR}/logger.c
        ${SOURCE_DIR}/manualmap.c
        ${SOURCE_DIR}/util.c
    )

    ## Test executable and link dependencies
    add_executable( ${PROJECT_NAME} ${TEST_SOURCES} )

    ## Add link directories
    target_link_directories(
        ${PROJECT_NAME}
        PRIVATE
        /usr/local/share/mingw64/lib
    )

    ## Link Dependencies
    target_link_libraries( ${PROJECT_NAME} psapi libcunit.a )

    set_target_properties(
        ${PROJECT_NAME}
	PROPERTIES
	LINK_STATIC_LIBRARIES TRUE
    )
endif()
