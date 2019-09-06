#
#   ObjectListModel CMake build script
#
#   Copyright Olivier Le Doeuff 2019
#

# ┌──────────────────────────────────────────────────────────────────┐
# │                       CMAKE PROPERTIES                           │
# └──────────────────────────────────────────────────────────────────┘

cmake_minimum_required(VERSION 3.11 FATAL_ERROR)

# DEPENDANCIES

set(GTEST_REPOSITORY "https://github.com/google/googletest" CACHE STRING "Repository of gtest")
set(GTEST_TAG "master" CACHE STRING "Git tag/branch of gtest")

# ┌──────────────────────────────────────────────────────────────────┐
# │                       PROJECT SETTINGS                           │
# └──────────────────────────────────────────────────────────────────┘

# OPTIONS

set(OBJLISTMODEL_VERSION_MAJOR 2)
set(OBJLISTMODEL_VERSION_MINOR 0)
set(OBJLISTMODEL_VERSION_PATCH 0)
if( EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/.git" )
    EXECUTE_PROCESS(
        COMMAND git describe --always
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        OUTPUT_VARIABLE OBJLISTMODEL_VERSION_TAG
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
endif()
if(NOT OBJLISTMODEL_VERSION_TAG)
    set( OBJLISTMODEL_VERSION_TAG 00000000 )
endif(NOT OBJLISTMODEL_VERSION_TAG)
set( OBJLISTMODEL_VERSION_TAG_HEX 0x${OBJLISTMODEL_VERSION_TAG})
set( OBJLISTMODEL_VERSION ${OBJLISTMODEL_VERSION_MAJOR}.${OBJLISTMODEL_VERSION_MINOR}.${OBJLISTMODEL_VERSION_PATCH} CACHE STRING "" FORCE )

# General
set( OBJLISTMODEL_PROJECT "ObjectListModel" CACHE STRING "Project Name")
set( OBJLISTMODEL_TARGET ${OBJLISTMODEL_PROJECT} CACHE STRING "ObjectListModel library name" )
set( OBJLISTMODEL_VERSION ${OBJLISTMODEL_VERSION_MAJOR}.${OBJLISTMODEL_VERSION_MINOR}.${OBJLISTMODEL_VERSION_PATCH} CACHE STRING "ObjectListModel current version, this is only decorative and will not configure any files" FORCE )


set( OBJLISTMODEL_USE_NAMESPACE ON CACHE BOOL "ObjectListModel should use namespace" )
set( OBJLISTMODEL_NAMESPACE "Olm" CACHE STRING "ObjectListModel namespace" )

set( OBJLISTMODEL_BUILD_SHARED OFF CACHE BOOL "Build as a shared library (ON) or as static (OFF)" )
set( OBJLISTMODEL_FOLDER_PREFIX ${OBJLISTMODEL_PROJECT} CACHE STRING "Prefix folder for all ObjectListModel generated targets in generated project (only decorative)" )

# Tests
set( OBJLISTMODEL_ENABLE_TESTS OFF CACHE BOOL "Create or not a target for test (compatible with CTests)" )
set( OBJLISTMODEL_TESTS_PREFIX ${OBJLISTMODEL_PROJECT} CACHE STRING "Prefix for all ObjectListModel tests" )

# Examples
set( OBJLISTMODEL_ENABLE_EXAMPLES OFF CACHE BOOL "Create or not a target for examples" )
set( OBJLISTMODEL_EXAMPLES_PREFIX ${OBJLISTMODEL_PROJECT} CACHE STRING "Prefix for all ObjectListModel tests" )

# Install
set( OBJLISTMODEL_ENABLE_INSTALL OFF CACHE BOOL "Create or not a target for install" )
set( OBJLISTMODEL_INSTALL_PREFIX ${OBJLISTMODEL_PROJECT} CACHE STRING "Prefix for all ObjectListModel headers in the install folder" )

# CREATE PROJECT

PROJECT( ${OBJLISTMODEL_PROJECT} VERSION ${OBJLISTMODEL_VERSION} LANGUAGES C CXX )
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)
set( CMAKE_CXX_STANDARD 11 )

# LOG OPTIONS

message("---------------- OBJLISTMODEL OPTIONS. ----------------")

#General
message(STATUS "OBJLISTMODEL_PROJECT          : " ${OBJLISTMODEL_PROJECT})
message(STATUS "OBJLISTMODEL_TARGET           : " ${OBJLISTMODEL_TARGET})
message(STATUS "OBJLISTMODEL_VERSION          : " ${OBJLISTMODEL_VERSION})
message(STATUS "OBJLISTMODEL_VERSION_TAG      : " ${OBJLISTMODEL_VERSION_TAG})
message(STATUS "OBJLISTMODEL_USE_NAMESPACE    : " ${OBJLISTMODEL_USE_NAMESPACE})
message(STATUS "OBJLISTMODEL_NAMESPACE        : " ${OBJLISTMODEL_NAMESPACE})
message(STATUS "OBJLISTMODEL_BUILD_SHARED     : " ${OBJLISTMODEL_BUILD_SHARED})
message(STATUS "OBJLISTMODEL_FOLDER_PREFIX    : " ${OBJLISTMODEL_FOLDER_PREFIX})

# Tests
message(STATUS "OBJLISTMODEL_ENABLE_TESTS     : " ${OBJLISTMODEL_ENABLE_TESTS})
if(OBJLISTMODEL_ENABLE_TESTS)
message(STATUS "OBJLISTMODEL_TESTS_PREFIX     : " ${OBJLISTMODEL_TESTS_PREFIX})
endif(OBJLISTMODEL_ENABLE_TESTS)

# Examples
message(STATUS "OBJLISTMODEL_ENABLE_EXAMPLES  : " ${OBJLISTMODEL_ENABLE_EXAMPLES})
if(OBJLISTMODEL_ENABLE_EXAMPLES)
message(STATUS "OBJLISTMODEL_EXAMPLES_PREFIX  : " ${OBJLISTMODEL_EXAMPLES_PREFIX})
endif(OBJLISTMODEL_ENABLE_EXAMPLES)

# Install
message(STATUS "OBJLISTMODEL_ENABLE_INSTALL   : " ${OBJLISTMODEL_ENABLE_INSTALL})
if(OBJLISTMODEL_ENABLE_INSTALL)
message(STATUS "OBJLISTMODEL_INSTALL_PREFIX   : " ${OBJLISTMODEL_INSTALL_PREFIX})
endif(OBJLISTMODEL_ENABLE_INSTALL)

message("---------------- DONE WITH OPTIONS. -----------------")
message("                                                     ")

# ┌──────────────────────────────────────────────────────────────────┐
# │                         QT5 CMAKE                                │
# └──────────────────────────────────────────────────────────────────┘

find_package(Qt5Core CONFIG REQUIRED)

# ┌──────────────────────────────────────────────────────────────────┐
# │                       DEPENDENCIES                               │
# └──────────────────────────────────────────────────────────────────┘

include(FetchContent)

# gtest
FetchContent_Declare(
    gtest
    GIT_REPOSITORY ${GTEST_REPOSITORY}
    GIT_TAG        ${GTEST_TAG}
)

# Is it a thing ??
set(GTEST_FOLDER_PREFIX "Tests/Dependencies/gtest" CACHE STRING "")

set(FETCHCONTENT_UPDATES_DISCONNECTED ON)
set(FETCHCONTENT_QUIET ON)

# ┌──────────────────────────────────────────────────────────────────┐
# │                          OBJLISTMODEL                              │
# └──────────────────────────────────────────────────────────────────┘

set(OBJLISTMODEL_SRCS_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(OBJLISTMODEL_INCS_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/src/include)
set(OBJLISTMODEL_PRIVATE_INCS_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/src/include/ObjectListModel)

# OBJLISTMODEL SOURCES
set(OBJLISTMODEL_SRCS

    # Utils
    ${OBJLISTMODEL_SRCS_FOLDER}/Utils.cpp
    ${OBJLISTMODEL_SRCS_FOLDER}/Version.cpp

    # Library
)

set(OBJLISTMODEL_INCS

    # -- Public -- #

    ${OBJLISTMODEL_INCS_FOLDER}/ObjectListModel.hpp

    # -- Private -- #

    # Utils
    ${OBJLISTMODEL_PRIVATE_INCS_FOLDER}/Export.hpp
    ${OBJLISTMODEL_PRIVATE_INCS_FOLDER}/Utils.hpp
    ${OBJLISTMODEL_PRIVATE_INCS_FOLDER}/Version.hpp

    # Library
    ${OBJLISTMODEL_PRIVATE_INCS_FOLDER}/ObjectList.hpp
    ${OBJLISTMODEL_PRIVATE_INCS_FOLDER}/SharedObjectList.hpp
)

# OBJLISTMODEL TARGET
if(OBJLISTMODEL_BUILD_SHARED)

    add_library(${OBJLISTMODEL_TARGET} SHARED ${OBJLISTMODEL_SRCS}
        ${OBJLISTMODEL_INCS}
        )
    target_compile_definitions(${OBJLISTMODEL_TARGET} PRIVATE "-DOBJLISTMODEL_DLL_EXPORT")

else(OBJLISTMODEL_BUILD_SHARED)

    add_library(${OBJLISTMODEL_TARGET} STATIC ${OBJLISTMODEL_SRCS}
        ${OBJLISTMODEL_INCS}
        )
    target_compile_definitions(${OBJLISTMODEL_TARGET} PUBLIC "-DOBJLISTMODEL_STATIC")

endif(OBJLISTMODEL_BUILD_SHARED)

# Set include directory
target_include_directories(${OBJLISTMODEL_TARGET} PUBLIC $<BUILD_INTERFACE:${OBJLISTMODEL_INCS_FOLDER}> $<INSTALL_INTERFACE:${OBJLISTMODEL_INSTALL_PREFIX}>)

# Avoid libnetcode.io on darwin for example.
set_target_properties(${OBJLISTMODEL_TARGET} PROPERTIES PREFIX "")

# Link Qt5
set_target_properties(${OBJLISTMODEL_TARGET} PROPERTIES AUTOMOC TRUE)
qt5_use_modules( ${OBJLISTMODEL_TARGET} Core Qml )

set_target_properties(${OBJLISTMODEL_TARGET} PROPERTIES FOLDER ${OBJLISTMODEL_FOLDER_PREFIX})

# Version
target_compile_definitions( ${OBJLISTMODEL_TARGET} PRIVATE -DOBJLISTMODEL_VERSION_MAJOR=${OBJLISTMODEL_VERSION_MAJOR} )
target_compile_definitions( ${OBJLISTMODEL_TARGET} PRIVATE -DOBJLISTMODEL_VERSION_MINOR=${OBJLISTMODEL_VERSION_MINOR} )
target_compile_definitions( ${OBJLISTMODEL_TARGET} PRIVATE -DOBJLISTMODEL_VERSION_PATCH=${OBJLISTMODEL_VERSION_PATCH} )
target_compile_definitions( ${OBJLISTMODEL_TARGET} PRIVATE -DOBJLISTMODEL_VERSION_TAG=${OBJLISTMODEL_VERSION_TAG} )
target_compile_definitions( ${OBJLISTMODEL_TARGET} PRIVATE -DOBJLISTMODEL_VERSION_TAG_HEX=${OBJLISTMODEL_VERSION_TAG_HEX} )

if(OBJLISTMODEL_USE_NAMESPACE)
    target_compile_definitions( ${OBJLISTMODEL_TARGET} PUBLIC -DOBJLISTMODEL_USE_NAMESPACE )
    target_compile_definitions( ${OBJLISTMODEL_TARGET} PUBLIC -DOBJLISTMODEL_NAMESPACE=${OBJLISTMODEL_NAMESPACE} )
endif(OBJLISTMODEL_USE_NAMESPACE)

# ┌──────────────────────────────────────────────────────────────────┐
# │                           TESTS                                  │
# └──────────────────────────────────────────────────────────────────┘

if(OBJLISTMODEL_ENABLE_TESTS)
    include(${CMAKE_CURRENT_SOURCE_DIR}/CTestLists.txt)
endif(OBJLISTMODEL_ENABLE_TESTS)

# ┌──────────────────────────────────────────────────────────────────┐
# │                         EXAMPLES                                 │
# └──────────────────────────────────────────────────────────────────┘

set(OBJLISTMODEL_EXAMPLES_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/examples)

if(OBJLISTMODEL_ENABLE_EXAMPLES)

    macro(objlist_example_add EXAMPLE_NAME)

        set(EXAMPLE_FULL_NAME ${OBJLISTMODEL_EXAMPLES_PREFIX}_${EXAMPLE_NAME})

        message(STATUS "Add Example: ${EXAMPLE_FULL_NAME}")

        add_executable            (${EXAMPLE_FULL_NAME} ${OBJLISTMODEL_EXAMPLES_FOLDER}/${EXAMPLE_NAME}.cpp
                                    ${OBJLISTMODEL_EXAMPLES_FOLDER}/include/FooExample.hpp)
        set_target_properties     (${EXAMPLE_FULL_NAME} PROPERTIES AUTOMOC TRUE)
        target_link_libraries     (${EXAMPLE_FULL_NAME} ${OBJLISTMODEL_TARGET})
        set_target_properties     (${EXAMPLE_FULL_NAME} PROPERTIES FOLDER ${OBJLISTMODEL_FOLDER_PREFIX}/Examples)
        target_include_directories(${EXAMPLE_FULL_NAME} PUBLIC ${OBJLISTMODEL_EXAMPLES_FOLDER}/include)

    endmacro()

    objlist_example_add(ObjectListExample)

endif(OBJLISTMODEL_ENABLE_EXAMPLES)

# ┌──────────────────────────────────────────────────────────────────┐
# │                         INSTALL                                  │
# └──────────────────────────────────────────────────────────────────┘

if(OBJLISTMODEL_ENABLE_INSTALL)
    include(${CMAKE_CURRENT_SOURCE_DIR}/CInstallLists.txt)
endif(OBJLISTMODEL_ENABLE_INSTALL)