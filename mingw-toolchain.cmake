## Set compilers
if     ( "${PROJECT_ARCHITECTURE}" STREQUAL "x86" )
    set( CMAKE_CXX_COMPILER "i686-w64-mingw32-g++" )
elseif ( "${PROJECT_ARCHITECTURE}" STREQUAL "x64" )
    set( CMAKE_CXX_COMPILER "x86_64-w64-mingw32-g++" )
endif()
