macro(objlist_example_add EXAMPLE_NAME)

  set(EXAMPLE_FULL_NAME ${QOLM_EXAMPLES_PREFIX}_${EXAMPLE_NAME})

  message(STATUS "Add Example: ${EXAMPLE_FULL_NAME}")

  add_executable            (${EXAMPLE_FULL_NAME} ${QOLM_EXAMPLES_FOLDER}/${EXAMPLE_NAME}.cpp
    ${QOLM_EXAMPLES_FOLDER}/include/FooExample.hpp)
  set_target_properties     (${EXAMPLE_FULL_NAME} PROPERTIES AUTOMOC TRUE)
  target_link_libraries     (${EXAMPLE_FULL_NAME} PRIVATE QOlm)
  set_target_properties     (${EXAMPLE_FULL_NAME} PROPERTIES FOLDER ${QOLM_FOLDER_PREFIX}/Examples)
  target_include_directories(${EXAMPLE_FULL_NAME} PUBLIC include/)

endmacro()

set(QOLM_EXAMPLES_FOLDER ${CMAKE_CURRENT_SOURCE_DIR})
objlist_example_add(QOlmExample)