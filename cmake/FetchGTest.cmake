include(FetchContent)

set(GTEST_REPOSITORY "https://github.com/google/googletest.git" CACHE STRING "googletest repository url")
set(GTEST_TAG "master" CACHE STRING "googletest git tag")

FetchContent_Declare(
  googletest
  GIT_REPOSITORY ${GTEST_REPOSITORY}
  GIT_TAG        ${GTEST_TAG}
  GIT_SHALLOW    1
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
option(INSTALL_GTEST "Enable installation of googletest. (Projects embedding googletest may want to turn this OFF.)" OFF)
FetchContent_MakeAvailable(googletest)

set_target_properties(gtest gtest_main gmock gmock_main PROPERTIES FOLDER "Dependencies/gtest")
target_compile_features(gtest PUBLIC cxx_std_11)
target_compile_features(gtest_main PUBLIC cxx_std_11)
target_compile_features(gmock PUBLIC cxx_std_11)
target_compile_features(gmock_main PUBLIC cxx_std_11)
