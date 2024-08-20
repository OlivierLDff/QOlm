# SPDX-License-Identifier: MIT
#
# SPDX-FileCopyrightText: Copyright (c) 2019-2023 Lars Melchior and contributors

# Heuristic to check if file have already been found
if(COMMAND CpmAddPackage)
  return()
endif()

if(QOLM_USE_LOCAL_CPM_FILE)
  message(STATUS "Looking for local CPM.cmake")
  find_file(CPM_LOCAL_FILE
    CPM.cmake
    PATH_SUFFIXES share/cpm/
  )

  if(EXISTS ${CPM_LOCAL_FILE})
    message(STATUS "Using local CPM.cmake: ${CPM_LOCAL_FILE}")
    include(${CPM_LOCAL_FILE})
    return()
  endif()
endif()

# Now the original from the release that can be found at
# https://github.com/cpm-cmake/CPM.cmake/releases

set(CPM_DOWNLOAD_VERSION 0.39.0)
set(CPM_HASH_SUM "66639bcac9dd2907b2918de466783554c1334446b9874e90d38e3778d404c2ef")

if(CPM_SOURCE_CACHE)
  set(CPM_DOWNLOAD_LOCATION "${CPM_SOURCE_CACHE}/cpm/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
elseif(DEFINED ENV{CPM_SOURCE_CACHE})
  set(CPM_DOWNLOAD_LOCATION "$ENV{CPM_SOURCE_CACHE}/cpm/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
else()
  set(CPM_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/cmake/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
endif()

# Expand relative path. This is important if the provided path contains a tilde (~)
get_filename_component(CPM_DOWNLOAD_LOCATION ${CPM_DOWNLOAD_LOCATION} ABSOLUTE)

file(DOWNLOAD
     https://github.com/cpm-cmake/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake
     ${CPM_DOWNLOAD_LOCATION} EXPECTED_HASH SHA256=${CPM_HASH_SUM}
)

include(${CPM_DOWNLOAD_LOCATION})
