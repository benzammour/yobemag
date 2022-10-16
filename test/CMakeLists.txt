set(TEST_NAME yobemag_test)

set(TEST_FILES
    sample_unit_test.c)

add_executable(${TEST_NAME} ${TEST_FILES})

target_link_libraries(${TEST_NAME} PRIVATE -lm -lcriterion)

add_custom_target(test COMMAND ./${TEST_NAME} DEPENDS ${TEST_NAME})
