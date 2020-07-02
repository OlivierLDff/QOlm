#
#   QOlm CTest script
#
#   Copyright Olivier Le Doeuff 2019
#

# We need QSignalSpy
find_package(Qt5Test CONFIG REQUIRED)

set(QOLM_TEST_TARGET ${QOLM_TESTS_PREFIX}_Tests)

set(QOLM_TEST_TARGET_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(QOLM_TEST_SRCS  ${QOLM_TEST_TARGET_DIR}/Tests.cpp
        ${QOLM_TEST_TARGET_DIR}/QOlmTests.cpp

        ${QOLM_TEST_TARGET_DIR}/include/Foo.hpp
        ${QOLM_TEST_TARGET_DIR}/include/FooList.hpp
    )

message(STATUS "Add Test: ${QOLM_TEST_TARGET}")

# Create the executable
add_executable(${QOLM_TEST_TARGET} ${QOLM_TEST_SRCS})
set_target_properties(${QOLM_TEST_TARGET} PROPERTIES AUTOMOC TRUE)
target_include_directories(${QOLM_TEST_TARGET} PUBLIC ${QOLM_TEST_TARGET_DIR}/include)
target_link_libraries(${QOLM_TEST_TARGET} PRIVATE ${QOLM_TARGET})
target_link_libraries(${QOLM_TEST_TARGET} PRIVATE Qt5::Test gtest)
set_target_properties(${QOLM_TEST_TARGET} PROPERTIES FOLDER ${QOLM_FOLDER_PREFIX}/Tests)

# Register the test
add_test(NAME ${QOLM_TEST_TARGET} COMMAND ${QOLM_TEST_TARGET})