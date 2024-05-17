cmake_minimum_required( VERSION 3.22.0 )

set( CMAKE_C_STANDARD_REQUIRED ON )
set( CMAKE_C_STANDARD          99 )

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib )

project( vpr-pidjeon )

## Main targets
add_subdirectory( src )

## Test targets
#add_subdirectory( test )
