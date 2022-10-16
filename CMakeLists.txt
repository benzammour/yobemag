cmake_minimum_required(VERSION 3.0)
project(YOBEMAG C)

set(PRODUCT_NAME yobemag)
string(TOUPPER ${PRODUCT_NAME} MSG_PREFIX)

include_directories(src)
set(SRC_FILES
    src/cpu.c
    src/main.c
    src/mmu.c
    src/lcd.c
    src/rom.c)

add_executable(${PRODUCT_NAME} ${SRC_FILES})

add_subdirectory(test)

message("[${MSG_PREFIX}] Using ${CMAKE_GENERATOR}")
message("[${MSG_PREFIX}] Using ${CMAKE_C_COMPILER} ${CMAKE_C_COMPILER_VERSION}")

if (CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "DEBUG" OR CMAKE_BUILD_TYPE STREQUAL "debug")
    message("[${MSG_PREFIX}] Using -g")
    target_compile_options(${PRODUCT_NAME} PUBLIC -g)
endif ()

target_compile_options(${PRODUCT_NAME} PUBLIC
    # Standard GCC/Clang warnings
    -Wall
    -Wextra
    -Werror
    -Wunused
    -Wshadow
    -Wcast-align
    -Wconversion
    -Wdouble-promotion
    -fstack-protector -Wstack-protector
    -Wredundant-decls
    -Wpacked
    -Wmissing-declarations
    -Wmissing-prototypes
    -Wstrict-prototypes
    -Wwrite-strings
    -Wcast-qual
    -Wundef
    -Wfloat-equal
    -Wstrict-overflow=4
    -Wswitch-enum
    -Wswitch-default
    -Wmissing-include-dirs
    -Wformat=2)

if (DEFINED OPTIMIZE)
    message("[${MSG_PREFIX}] Using -O${OPTIMIZE}")
    # Use the same optimization level for build and test
    add_compile_options(-O${OPTIMIZE})
endif ()

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_C_COMPILER_ID STREQUAL "Clang")
    target_compile_options(${PRODUCT_NAME} PUBLIC
        -Weverything
        -Wno-declaration-after-statement
        -Wno-padded
        -Wno-gnu-designator
        -ferror-limit=0)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_C_COMPILER_ID STREQUAL "GNU")
    target_compile_options(${PRODUCT_NAME} PUBLIC
        # Standard GCC warnings: https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html
        -Wlogical-op
        -Warray-bounds=2
        -Wjump-misses-init
        -Wunsafe-loop-optimizations
        -Wno-aggressive-loop-optimizations
        -Wjump-misses-init
        -Wtrampolines
        -Wsuggest-attribute=cold
        -Wsuggest-attribute=malloc
        -Wsuggest-attribute=pure
        -Wsuggest-attribute=const
        -Wsuggest-attribute=noreturn
        -Wsuggest-attribute=format
        -fmax-errors=0)
endif ()

set_property(TARGET ${PRODUCT_NAME} PROPERTY C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

target_link_libraries(${PRODUCT_NAME} PUBLIC -lm -lSDL2 -pthread)
