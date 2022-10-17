set(TEST_NAME ${PRODUCT_NAME}_test)

set(TEST_FILES
    sample_unit_test.c)

set(TEST_SOURCES ${TEST_FILES} ${SRC_FILES})
list(REMOVE_ITEM TEST_SOURCES "${PROJECT_SOURCE_DIR}/src/main.c")
add_executable(${TEST_NAME} ${TEST_SOURCES})

if (DEFINED COVERAGE)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_C_COMPILER_ID STREQUAL "GNU")
        message("[${UPPER_PRODUCT_NAME}] Adding coverage flags")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -fprofile-arcs -ftest-coverage")
    else ()
        message("[${MSG_PREFIX}] GCC is required to output coverage reports")
    endif ()
endif ()

target_link_libraries(${TEST_NAME} PRIVATE -lm -lSDL2 -pthread -lcriterion)

add_custom_target(test COMMAND ./${TEST_NAME} DEPENDS ${TEST_NAME})
