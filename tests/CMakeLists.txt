#
#   QOlm CTest script
#
#   Copyright Olivier Le Doeuff 2019
#

# We need QSignalSpy
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS
    Quick
    QuickTest
    Test
)

set(QOLM_TEST_TARGET ${QOLM_TESTS_PREFIX}_Tests)

set(QOLM_TEST_TARGET_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(QOLM_TEST_SRCS  ${QOLM_TEST_TARGET_DIR}/Tests.cpp
        ${QOLM_TEST_TARGET_DIR}/QOlmTests.cpp
        ${QOLM_TEST_TARGET_DIR}/QOlmTests.cpp

        ${QOLM_TEST_TARGET_DIR}/include/Foo.hpp
        ${QOLM_TEST_TARGET_DIR}/include/FooList.hpp
    )

message(STATUS "Add Test: ${QOLM_TEST_TARGET}")

# Create the executable
add_executable(${QOLM_TEST_TARGET} ${QOLM_TEST_SRCS})
set_target_properties(${QOLM_TEST_TARGET} PROPERTIES AUTOMOC TRUE)
target_include_directories(${QOLM_TEST_TARGET} PRIVATE ${QOLM_TEST_TARGET_DIR}/include)
target_link_libraries(${QOLM_TEST_TARGET} PRIVATE ${QOLM_TARGET})
target_link_libraries(${QOLM_TEST_TARGET} PRIVATE Qt::Test gtest gmock)
set_target_properties(${QOLM_TEST_TARGET} PROPERTIES FOLDER ${QOLM_FOLDER_PREFIX}/Tests)

# Register the test
add_test(NAME ${QOLM_TEST_TARGET} COMMAND ${QOLM_TEST_TARGET})

#QOLM Tests QML API

set(QOLM_TEST_TARGET_QML ${QOLM_TESTS_PREFIX}_TestsQml)
set(QOLM_TEST_TARGET_QML_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(QOLM_TEST_QML_SRCS  ${QOLM_TEST_TARGET_QML_DIR}/QOlmTestsQml.cpp)

message(STATUS "Add Test: ${QOLM_TEST_TARGET_QML}")

# Create the executable
add_executable(${QOLM_TEST_TARGET_QML} ${QOLM_TEST_QML_SRCS})
set_target_properties(${QOLM_TEST_TARGET_QML} PROPERTIES AUTOMOC TRUE AUTORCC TRUE)
target_include_directories(${QOLM_TEST_TARGET_QML} PRIVATE ${QOLM_TEST_TARGET_QML_DIR}/include)
target_link_libraries(${QOLM_TEST_TARGET_QML} PRIVATE ${QOLM_TARGET})
target_link_libraries(${QOLM_TEST_TARGET_QML} PRIVATE Qt::QuickTest Qt::Test Qt::Quick)
set_target_properties(${QOLM_TEST_TARGET_QML} PROPERTIES FOLDER ${QOLM_FOLDER_PREFIX}/Tests)

# Register the test
add_test(NAME ${QOLM_TEST_TARGET_QML} COMMAND "$<TARGET_FILE:${QOLM_TEST_TARGET_QML}>" -input "${CMAKE_CURRENT_SOURCE_DIR}")
