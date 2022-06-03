# Set compilers
set(CMAKE_C_COMPILER i686-w64-mingw32-gcc-posix)

# Set compile flags
set(CMAKE_C_FLAGS "-std=c17 -O2 -s -static -Wall -Wextra -Wshadow -Wpedantic -Wconversion")
