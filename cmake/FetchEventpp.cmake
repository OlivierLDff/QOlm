include(FetchContent)

set(EVENTPP_REPOSITORY "https://github.com/OlivierLDff/eventpp.git" CACHE STRING "eventpp repository url")
set(EVENTPP_TAG "cmake-cpp11-feature" CACHE STRING "eventpp git tag")

FetchContent_Declare(
  eventpp
  GIT_REPOSITORY ${EVENTPP_REPOSITORY}
  GIT_TAG        ${EVENTPP_TAG}
)

FetchContent_MakeAvailable(eventpp)