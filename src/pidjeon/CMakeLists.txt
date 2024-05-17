project( pidjeon
         VERSION        2.0.1
         LANGUAGES      C
)

include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )
include_directories( ${CMAKE_SOURCE_DIR}/src )

add_executable(
    ${PROJECT_NAME}
         main.c
         parser.c
)

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
        OUTPUT_NAME ${PROJECT_NAME}
        PREFIX "vpr-"
        SUFFIX "-${PROJECT_ARCHITECTURE}.exe"
)

target_compile_options(
    ${PROJECT_NAME}
    PRIVATE 
        $<$<CXX_COMPILER_ID:MSVC>:/W4>
        $<$<CXX_COMPILER_ID:MSVC>:/WX>
        $<$<CXX_COMPILER_ID:MSVC>:/O2>
        $<$<CXX_COMPILER_ID:GNU>:-Ofast>
        $<$<CXX_COMPILER_ID:GNU>:-Wall>
        $<$<CXX_COMPILER_ID:GNU>:-Wextra>
        $<$<CXX_COMPILER_ID:GNU>:-Werror>
        $<$<CXX_COMPILER_ID:GNU>:-Wshadow>
#       $<$<CXX_COMPILER_ID:GNU>:-Wpedantic>
        $<$<CXX_COMPILER_ID:GNU>:-Wconversion>
        $<$<CXX_COMPILER_ID:GNU>:-Wno-unused-value>
        $<$<CXX_COMPILER_ID:GNU>:-Wno-cast-function-type>
        $<$<CXX_COMPILER_ID:GNU>:-Wno-missing-field-initializers>
        $<$<CXX_COMPILER_ID:GNU>:-fPIE>
        $<$<CXX_COMPILER_ID:GNU>:-fno-ident>
        $<$<CXX_COMPILER_ID:GNU>:-ffast-math>
        $<$<CXX_COMPILER_ID:GNU>:-funroll-loops>
        $<$<CXX_COMPILER_ID:GNU>:-fvisibility=hidden>
        $<$<CXX_COMPILER_ID:AppleClang>:-Ofast>
        $<$<CXX_COMPILER_ID:AppleClang>:-Wall>
        $<$<CXX_COMPILER_ID:AppleClang>:-Wextra>
        $<$<CXX_COMPILER_ID:AppleClang>:-Werror>
        $<$<CXX_COMPILER_ID:AppleClang>:-Wshadow>
#       $<$<CXX_COMPILER_ID:AppleClang>:-Wpedantic>
        $<$<CXX_COMPILER_ID:AppleClang>:-Wconversion>
        $<$<CXX_COMPILER_ID:AppleClang>:-ffunction-sections>
        $<$<CXX_COMPILER_ID:AppleClang>:-ffast-math>
        $<$<CXX_COMPILER_ID:AppleClang>:-funroll-loops>
        $<$<CXX_COMPILER_ID:AppleClang>:-fPIE>
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
        psapi
        inject_static
        common_static
)
