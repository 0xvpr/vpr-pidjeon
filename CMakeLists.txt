cmake_minimum_required(VERSION 3.8.0)

## Initialize project
project(pidjeon C)
set(TARGET pidjeon)

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

# Set architecture suffix
set_target_properties(
    ${TARGET}
    PROPERTIES
    OUTPUT_NAME ${TARGET}_${ARCH}
    SUFFIX ".exe"
)

# -------------- DUMMY -------------- #

project(dummy CXX)
set(DUMMY dummy)
set(DUMMY_DIR tests)
set(DUMMY_SOURCES ${DUMMY_DIR}/dummy.cpp)

## Dummy executable
add_executable(${DUMMY} ${DUMMY_SOURCES}) 

# Set architecture suffix
set_target_properties(
    ${DUMMY}
    PROPERTIES
    OUTPUT_NAME ${DUMMY}_${ARCH}
    SUFFIX ".exe"
)

# ------------- PAYLOAD ------------- #

project(payload CXX)
set(PAYLOAD payload)
set(PAYLOAD_DIR tests)
set(PAYLOAD_SOURCES ${PAYLOAD_DIR}/payload.cpp)

## Dummy executable
add_library(${PAYLOAD} SHARED ${PAYLOAD_SOURCES}) 

# Set architecture suffix
set_target_properties(
    ${PAYLOAD}
    PROPERTIES
    OUTPUT_NAME ${PAYLOAD}_${ARCH}
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
	include_directories(${TESTS} ${CMAKE_CURRENT_BINARY_DIR}/tests)

	## Source directories
	set(TESTS_DIR tests)

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
