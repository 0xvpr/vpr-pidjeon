cmake_minimum_required(VERSION 3.8.2)

project(Payload_Injector)
set(TARGET payload_injector)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 20)

if (MSVC)
    # Compiler flags
    # Defines
endif()

set(SRC_DIR src)
set(INCLUDE_DIR include)

set(SOURCES
  ${SRC_DIR}/main.c
  ${SRC_DIR}/util.c
  ${SRC_DIR}/parser.c
  ${SRC_DIR}/injector.c
  ${SRC_DIR}/manualmap.c
  ${INCLUDE_DIR}/util.h
  ${INCLUDE_DIR}/parser.h
  ${INCLUDE_DIR}/injector.h
  ${INCLUDE_DIR}/manualmap.h
)

include_directories(TARGET include)
add_executable(${TARGET} ${SOURCES})
