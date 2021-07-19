# How can I use this ?
#
# Well, license here is pretty "super-open", not even copy-left or copy-right,
# basically its close to the WTFPL, just use it as you want, as it's the most practical to you :
#
# * If you want to use it as GIT submodule and compile it in your app, do it.
# * If you prefer separate project as a shared library, do it.
# * If you need to modify the project to be able to integrate in you app (opensource or not), do it.
# * If you want to share you work on the library, thanks a lot, but if you don't, no problem.
# * If you think about some weird case I didn't talk about, well, do whatever you want, I don't need to know about it.
# * If you like it so much you want to spread the word, thank you, you're welcome.
#
# Enjoy !

cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

set(QOLM_MAIN_PROJECT OFF)
if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  set(QOLM_MAIN_PROJECT ON)
endif()

# ───── OPTIONS ─────

# General
set(QOLM_PROJECT "QOlm" CACHE STRING "Project Name")
set(QOLM_TARGET ${QOLM_PROJECT} CACHE STRING "QOlm library name")
set(QOLM_VERBOSE OFF CACHE BOOL "QOlm log configuration")

set(QOLM_BUILD_SHARED OFF CACHE BOOL "Build as a shared library (ON) or as static (OFF)")
set(QOLM_FOLDER_PREFIX ${QOLM_PROJECT} CACHE STRING "Prefix folder for all QOlm generated targets in generated project (only decorative)")

# Tests
set(QOLM_ENABLE_TESTS OFF CACHE BOOL "Create or not a target for test (compatible with CTests)")
set(QOLM_TESTS_PREFIX ${QOLM_PROJECT} CACHE STRING "Prefix for all QOlm tests")

# Examples
set(QOLM_ENABLE_EXAMPLES OFF CACHE BOOL "Create or not a target for examples")
set(QOLM_EXAMPLES_PREFIX ${QOLM_PROJECT} CACHE STRING "Prefix for all QOlm tests")

set(QOLM_ENABLE_INSTALL ${QOLM_MAIN_PROJECT} CACHE BOOL "Generate the install target")

# ───── VERSION ─────

set(QOLM_VERSION_MAJOR 3)
set(QOLM_VERSION_MINOR 1)
set(QOLM_VERSION_PATCH 4)
if( EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/.git")
  EXECUTE_PROCESS(
    COMMAND git rev-parse --short HEAD
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    OUTPUT_VARIABLE QOLM_VERSION_TAG
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
endif()
if(NOT QOLM_VERSION_TAG)
  set(QOLM_VERSION_TAG 00000000)
endif()
set(QOLM_VERSION_TAG_HEX 0x${QOLM_VERSION_TAG})
set(QOLM_VERSION ${QOLM_VERSION_MAJOR}.${QOLM_VERSION_MINOR}.${QOLM_VERSION_PATCH} CACHE STRING "" FORCE)
set(QOLM_VERSION ${QOLM_VERSION_MAJOR}.${QOLM_VERSION_MINOR}.${QOLM_VERSION_PATCH} CACHE STRING "QOlm current version, this is only decorative and will not configure any files" FORCE)

# ───── PROJECT ─────

project(${QOLM_PROJECT} VERSION ${QOLM_VERSION} LANGUAGES CXX)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# ───── CONFIG ─────

if(QOLM_VERBOSE)

  message(STATUS "---------------- ${QOLM_PROJECT} ${QOLM_VERSION} OPTIONS. ----------------")

  #General
  message(STATUS "QOLM_PROJECT          : " ${QOLM_PROJECT})
  message(STATUS "QOLM_TARGET           : " ${QOLM_TARGET})
  message(STATUS "QOLM_VERSION          : " ${QOLM_VERSION})
  message(STATUS "QOLM_VERSION_TAG      : " ${QOLM_VERSION_TAG})
  message(STATUS "QOLM_FOLDER_PREFIX    : " ${QOLM_FOLDER_PREFIX})
  message(STATUS "QOLM_BUILD_SHARED     : " ${QOLM_BUILD_SHARED})
  message(STATUS "QOLM_ENABLE_INSTALL   : " ${QOLM_ENABLE_INSTALL})

  # Tests
  message(STATUS "QOLM_ENABLE_TESTS     : " ${QOLM_ENABLE_TESTS})
  if(QOLM_ENABLE_TESTS)
    message(STATUS "QOLM_TESTS_PREFIX     : " ${QOLM_TESTS_PREFIX})
  endif()

  # Examples
  message(STATUS "QOLM_ENABLE_EXAMPLES  : " ${QOLM_ENABLE_EXAMPLES})
  if(QOLM_ENABLE_EXAMPLES)
    message(STATUS "QOLM_EXAMPLES_PREFIX  : " ${QOLM_EXAMPLES_PREFIX})
  endif()

  message(STATUS "---------------- DONE WITH OPTIONS. -----------------")

endif()

# ───── DEPENDENCIES ─────

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Qml REQUIRED)

# ───── LIBRARY ─────

set(QOLM_HDRS
  include/QOlm/Details/Export.hpp
  include/QOlm/Details/QOlmBase.hpp
  include/QOlm/QOlm.hpp
)

set(QOLM_SRCS ${QOLM_HDRS}
  src/QOlmBase.cpp
)

if(QOLM_BUILD_SHARED)
  add_library(${QOLM_TARGET} SHARED ${QOLM_SRCS})
  target_compile_definitions(${QOLM_TARGET} PRIVATE "-DQOLM_DLL_EXPORT")
else()
  add_library(${QOLM_TARGET} STATIC  ${QOLM_SRCS})
  target_compile_definitions(${QOLM_TARGET} PUBLIC "-DQOLM_STATIC")
endif()

add_library(${QOLM_TARGET}::${QOLM_TARGET} ALIAS ${QOLM_TARGET})

target_include_directories(${QOLM_TARGET} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
target_compile_features(${QOLM_TARGET} PUBLIC cxx_std_17)
target_link_libraries(${QOLM_TARGET} PUBLIC Qt::Core Qt::Qml)

set_target_properties(${QOLM_TARGET} PROPERTIES FOLDER ${QOLM_FOLDER_PREFIX})
source_group("QOlm" FILES ${QOLM_SRCS})
set_target_properties(${QOLM_TARGET} PROPERTIES
  AUTOMOC TRUE
)

# ───── LIBRARY TESTS ─────

if(QOLM_ENABLE_TESTS)
  include(cmake/FetchGTest.cmake)
  enable_testing()
  add_subdirectory(tests)
endif()

# ───── LIBRARY EXAMPLES ─────

if(QOLM_ENABLE_EXAMPLES)
  add_subdirectory(examples)
endif()

# ───── LOG BUILD COMMAND ─────

if(QOLM_MAIN_PROJECT)
  include(cmake/PrintConfiguration.cmake)
endif()

# ───── INSTALLATION ─────

if(QOLM_ENABLE_INSTALL)
  include(GNUInstallDirs)
  install(TARGETS ${QOLM_TARGET} EXPORT ${QOLM_TARGET}Targets)

  foreach(HDR_FILE ${QOLM_HDRS})
    get_filename_component(HDR_DIRECTORY ${HDR_FILE} DIRECTORY)
    get_filename_component(ABSOLUTE_HDR_DIRECTORY ${HDR_DIRECTORY} ABSOLUTE)
    file(RELATIVE_PATH RELATIVE_HDR_PATH ${CMAKE_CURRENT_SOURCE_DIR}/include ${ABSOLUTE_HDR_DIRECTORY})
    install(FILES ${HDR_FILE} DESTINATION include/${RELATIVE_HDR_PATH})
  endforeach()

  export(EXPORT ${QOLM_TARGET}Targets
    FILE "${CMAKE_CURRENT_BINARY_DIR}/${QOLM_TARGET}/${QOLM_TARGET}Targets.cmake"
    NAMESPACE ${QOLM_TARGET}::
  )

  configure_file(cmake/${QOLM_TARGET}Config.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/${QOLM_TARGET}/${QOLM_TARGET}Config.cmake"
    @ONLY
  )

  include(CMakePackageConfigHelpers)
  write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${QOLM_TARGET}/${QOLM_TARGET}ConfigVersion.cmake"
    VERSION ${QOLM_VERSION}
    COMPATIBILITY AnyNewerVersion
  )

  set(ConfigPackageLocation lib/cmake/${QOLM_TARGET})
  install(EXPORT ${QOLM_TARGET}Targets
    FILE
    ${QOLM_TARGET}Targets.cmake
    NAMESPACE
    ${QOLM_TARGET}::
    DESTINATION
    ${ConfigPackageLocation}
  )
  install(
    FILES
      "${CMAKE_CURRENT_BINARY_DIR}/${QOLM_TARGET}/${QOLM_TARGET}Config.cmake"
      "${CMAKE_CURRENT_BINARY_DIR}/${QOLM_TARGET}/${QOLM_TARGET}ConfigVersion.cmake"
    DESTINATION
      ${ConfigPackageLocation}
  )
endif()
