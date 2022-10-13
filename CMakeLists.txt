cmake_minimum_required(VERSION 3.0)
project(YOBEMAG C)

set(PRODUCT_NAME yobemag)
string(TOUPPER ${PRODUCT_NAME} MSG_PREFIX)
set(CMAKE_C_STANDARD 17)

message("[${MSG_PREFIX}] Using ${CMAKE_GENERATOR}")

if (CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "DEBUG" OR CMAKE_BUILD_TYPE STREQUAL "debug")
    message("[${MSG_PREFIX}] Using -g")
    add_compile_options(-g)
endif()

add_compile_options(
        -std=c17

        ### WARNINGS
        # Standard GCC warnings: https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html
        -Wall
        -Wextra
        -Werror
        -Wunused
        -Wshadow
        -Wcast-align
        -Wconversion
        -Wlogical-op
        -Wdouble-promotion
        -fstack-protector -Wstack-protector
        -Wredundant-decls
        -Wpacked
        -Wmissing-declarations
        -Wmissing-prototypes
        -Wstrict-prototypes
        -Wno-aggressive-loop-optimizations
        -Wjump-misses-init
        -Wwrite-strings
        -Wcast-qual
        -Wundef
        -funsafe-loop-optimizations -Wunsafe-loop-optimizations
        -Wfloat-equal
        -Wtrampolines
        -Warray-bounds=2
        -Wstrict-overflow=4
        -Wswitch-enum
        -Wswitch-default
        -Wmissing-include-dirs
        -Wformat=2
)

message("[${MSG_PREFIX}] Using -O${OPTIMIZE}")
add_compile_options(-O${OPTIMIZE})

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_C_COMPILER_ID STREQUAL "Clang")
    message("[${MSG_PREFIX}] Using clang")
    add_compile_options(
            -Weverything
            -Wno-unknown-warning-option
            -Wno-format-nonliteral
            -ferror-limit=0
    )
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_C_COMPILER_ID STREQUAL "GNU")
    message("[${MSG_PREFIX}] Using gcc")
    add_compile_options(
            ### WARNINGS
            # Standard GCC warnings: https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html
            -Werror
            -Wlogical-op
            -Wjump-misses-init
            -Wunsafe-loop-optimizations
            -Wtrampolines
            -Wsuggest-attribute=cold
            -Wsuggest-attribute=malloc
            -Wsuggest-attribute=pure
            -Wsuggest-attribute=const
            -Wsuggest-attribute=noreturn
            -Wsuggest-attribute=format
            -Wformat=1
            -fmax-errors=0
    )
endif()

include_directories(src)
file(GLOB_RECURSE SRC_FILES "src/*.c")

add_executable(${PRODUCT_NAME} ${SRC_FILES})

target_link_libraries(${PRODUCT_NAME} PUBLIC -lm -lSDL2 -pthread)
