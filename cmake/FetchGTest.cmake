include(${CMAKE_CURRENT_LIST_DIR}/CPM.cmake)

set(GTEST_REPOSITORY "https://github.com/OlivierLDff/googletest.git" CACHE STRING "googletest repository url")
set(GTEST_TAG "v1.14.0" CACHE STRING "googletest git tag")

CPMAddPackage(
  NAME GTest
  GIT_REPOSITORY ${GTEST_REPOSITORY}
  GIT_TAG        ${GTEST_TAG}
  OPTIONS "gtest_force_shared_crt ON"
  OPTIONS "BUILD_GMOCK OFF"
  OPTIONS "INSTALL_GTEST OFF"
)
