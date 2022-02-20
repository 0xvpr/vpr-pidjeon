# Set compilers
set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc-posix)
set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++-posix)

# Set compile flags
set(CMAKE_C_FLAGS "-std=c99 -O2 -s -static -Wall -Wextra -Wshadow -Wpedantic -Wconversion")
set(CMAKE_CXX_FLAGS "-std=c++17 -O2 -s -static -Wall -Wextra -Wshadow -Wpedantic -Wconversion")
