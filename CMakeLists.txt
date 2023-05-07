cmake_minimum_required( VERSION 3.8.0 )

project( vpr-pidjeon )

## Main targets
add_subdirectory( src/pidjeon )
add_subdirectory( src/payload )
add_subdirectory( src/dummy )

## Test targets
add_subdirectory( test )
