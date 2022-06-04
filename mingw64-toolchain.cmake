# Set compilers
set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc-posix)

# Set compile flags
set(CMAKE_C_FLAGS "-std=c17 -O2 -s -Wall -Wextra -Wshadow -Wpedantic -Wconversion -ffast-math -fvisibility=hidden -funroll-loops -fPIE")
