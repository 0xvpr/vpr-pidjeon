cmake_minimum_required(VERSION 3.8.0)

# ------------- Wrapper ------------- #
if (${ARCH} STREQUAL "x64")
    project(wrapper C)
    set(TARGET wrapper)
    set(EXE_NAME vpr-pidjeon)

    ## Set output directory
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

    ## Include directories
    include_directories(${TARGET} ${CMAKE_SOURCE_DIR}/include)
    set(INCLUDE_DIR include)

    ## Source directories
    set(SOURCE_DIR src)

    ## Add CXX sources
    set(SOURCES
        # CXX sources
        ${SOURCE_DIR}/mem.c
        ${SOURCE_DIR}/util.c
        ${SOURCE_DIR}/wrapper.c
        # CXX includes
        ${INCLUDE_DIR}/mem.h
        ${INCLUDE_DIR}/pe32.h
        ${INCLUDE_DIR}/util.h
    )

    ## Application executable and link dependencies
    add_executable(${TARGET} ${SOURCES})

    ## Link Dependencies
    target_link_libraries(${TARGET} psapi)

    ## Set architecture suffix
    set_target_properties(
        ${TARGET}
        PROPERTIES
        OUTPUT_NAME ${EXE_NAME}
        SUFFIX ".exe"
    )
endif()

# --------------- vpr-pidjeon --------------- #

## Initialize project
project(vpr-pidjeon C)
set(TARGET vpr-pidjeon)
set(EXE_NAME vpr-pidjeon)

## Set output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

## Include directories
include_directories(${TARGET} ${CMAKE_SOURCE_DIR}/include)
set(INCLUDE_DIR include)

## Source directories
set(SOURCE_DIR src)

## Add CXX sources
set(SOURCES
    # CXX sources
    ${SOURCE_DIR}/main.c
    ${SOURCE_DIR}/crt.c
    ${SOURCE_DIR}/injector.c
    ${SOURCE_DIR}/loadlibrary.c
    ${SOURCE_DIR}/logger.c
    ${SOURCE_DIR}/manualmap.c
    ${SOURCE_DIR}/mem.c
    ${SOURCE_DIR}/parser.c
    ${SOURCE_DIR}/util.c
    # CXX includes
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
add_executable(${TARGET} ${SOURCES})

## Set architecture suffix
set_target_properties(
    ${TARGET}
    PROPERTIES
    OUTPUT_NAME ${EXE_NAME}-${ARCH}
    SUFFIX ".exe"
)

# -------------- DUMMY -------------- #

project(dummy C)
set(DUMMY dummy)
set(DUMMY_DIR src/tests)
set(DUMMY_SOURCES ${DUMMY_DIR}/dummy.c)

## Dummy executable
add_executable(${DUMMY} ${DUMMY_SOURCES}) 

## Set architecture suffix
set_target_properties(
    ${DUMMY}
    PROPERTIES
    OUTPUT_NAME ${DUMMY}-${ARCH}
    SUFFIX ".exe"
)

# ------------- PAYLOAD ------------- #

project(payload C)
set(PAYLOAD payload)
set(PAYLOAD_DIR src/tests)
set(PAYLOAD_SOURCES ${PAYLOAD_DIR}/payload.c)

## Dummy executable
add_library(${PAYLOAD} SHARED ${PAYLOAD_SOURCES}) 

## Set architecture suffix
set_target_properties(
    ${PAYLOAD}
    PROPERTIES
    OUTPUT_NAME ${PAYLOAD}-${ARCH}
    PREFIX ""
    SUFFIX ".dll"
)

# ------------- TESTING ------------- #

if (${ARCH} STREQUAL "x64")
	## Add testing support
	project(run-tests C)
	set(TESTS run-tests)

	## Include directories
	include_directories(${TESTS} /usr/local/share/mingw64/include)
	include_directories(${TESTS} ${CMAKE_CURRENT_BINARY_DIR}/include)
	include_directories(${TESTS} ${CMAKE_CURRENT_BINARY_DIR}src/tests)

	## Source directories
	set(TESTS_DIR src/tests)

	## Add CXX sources
	set(TEST_SOURCES
	    # CXX sources
	    ${TESTS_DIR}/test.c
	    ${SOURCE_DIR}/crt.c
	    ${SOURCE_DIR}/injector.c
	    ${SOURCE_DIR}/loadlibrary.c
	    ${SOURCE_DIR}/logger.c
	    ${SOURCE_DIR}/manualmap.c
	    ${SOURCE_DIR}/mem.c
	    ${SOURCE_DIR}/parser.c
	    ${SOURCE_DIR}/util.c
	    # CXX includes
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

	## Test executable and link dependencies
	add_executable(${TESTS} ${TEST_SOURCES}) 

	## Add link directories
	target_link_directories(${TESTS} PRIVATE /usr/local/share/mingw64/lib)

	## Link Dependencies
	target_link_libraries(${TESTS} cunit)
endif()
