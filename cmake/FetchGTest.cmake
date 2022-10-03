include(FetchContent)

set(GTEST_REPOSITORY "https://github.com/OlivierLDff/googletest.git" CACHE STRING "googletest repository url")
set(GTEST_TAG "release-1.12.1" CACHE STRING "googletest git tag")

FetchContent_Declare(
  googletest
  GIT_REPOSITORY ${GTEST_REPOSITORY}
  GIT_TAG        ${GTEST_TAG}
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
option(INSTALL_GTEST "Enable installation of googletest. (Projects embedding googletest may want to turn this OFF.)" OFF)
FetchContent_MakeAvailable(googletest)

set_target_properties(gtest gtest_main gmock gmock_main PROPERTIES FOLDER "Dependencies/gtest")
