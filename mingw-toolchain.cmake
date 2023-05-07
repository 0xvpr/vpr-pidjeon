## Set compilers
if     ( "${PROJECT_ARCHITECTURE}" STREQUAL "x86" )
    set( CMAKE_C_COMPILER "i686-w64-mingw32-gcc-posix" )
elseif ( "${PROJECT_ARCHITECTURE}" STREQUAL "x64" )
    set( CMAKE_C_COMPILER "x86_64-w64-mingw32-gcc-posix" )
endif()

## Set compile flags
set( CMAKE_C_FLAGS "-std=c99 -O2 -s -Wall -Wextra -Wshadow -Wconversion -fno-ident -ffast-math -fvisibility=hidden -funroll-loops -fPIE" )
