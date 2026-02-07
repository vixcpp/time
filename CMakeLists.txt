#  @file CMakeLists.txt
#  @author Gaspard Kirira
#
#  Copyright 2026, Gaspard Kirira.
#  All rights reserved.
#  https://github.com/vixcpp/time
#
#  Use of this source code is governed by a MIT license
#  that can be found in the LICENSE file.
#
# ====================================================================
# Vix.cpp - Time Module
# ====================================================================
# Purpose:
#   Time and date utilities for Vix.cpp.
#   Provides a simple, explicit API for working with:
#     - Date (facade, Node/Python-like)
#     - DateTime
#     - Timestamp
#     - Duration
#     - Parsing/formatting helpers
#
#   Builds as STATIC when sources exist, otherwise as a header-only
#   INTERFACE target.
#
# Public Targets:
#   - vix_time  : The actual library target (STATIC or INTERFACE)
#   - vix::time : Namespaced alias for consumers
#
# Public Dependencies:
#   - (none for v1)  -> keep it minimal, chrono-based
#
# Installation/Export:
#   Installs into the umbrella export-set `VixTargets`.
# ====================================================================

cmake_minimum_required(VERSION 3.20)
project(vix_time VERSION 0.1.0 LANGUAGES CXX)

include(GNUInstallDirs)

# Global settings
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# --------------------------------------------------------------------
# Umbrella build policy:
# When building inside the Vix umbrella, dependencies are managed by the
# umbrella (add_subdirectory order + options). Time must not FetchContent
# nor add sibling subdirectories in umbrella mode.
# --------------------------------------------------------------------
# (No deps for now, but keep the policy block for consistency.)
if (DEFINED VIX_UMBRELLA_BUILD AND VIX_UMBRELLA_BUILD)
  # nothing to enforce yet
endif()

# Sources discovery
file(GLOB_RECURSE TIME_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# ======================================================
# Target (STATIC if sources exist, else header-only)
# ======================================================

# STATIC
if (TIME_SOURCES)
  message(STATUS "[time] Building STATIC library with detected sources.")

  add_library(vix_time STATIC ${TIME_SOURCES})
  add_library(vix::time ALIAS vix_time)

  target_compile_features(vix_time PUBLIC cxx_std_20)

  target_include_directories(vix_time
    PUBLIC
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  )

  if (TARGET vix_warnings)
    target_link_libraries(vix_time PUBLIC vix_warnings)
  endif()

  if (VIX_ENABLE_SANITIZERS AND TARGET vix_sanitizers)
    target_link_libraries(vix_time PUBLIC vix_sanitizers)
  endif()

  set_target_properties(vix_time PROPERTIES
    OUTPUT_NAME vix_time
    VERSION ${PROJECT_VERSION}
    SOVERSION 0
    EXPORT_NAME time
  )

  install(TARGETS vix_time
    EXPORT VixTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  )

  # In standalone builds, install headers too.
  if (NOT (DEFINED VIX_UMBRELLA_BUILD AND VIX_UMBRELLA_BUILD))
    install(DIRECTORY include/
      DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
      FILES_MATCHING PATTERN "*.hpp" PATTERN "*.h"
    )
  endif()

# HEADER-ONLY
else()
  message(STATUS "[time] Building HEADER-ONLY library (no sources).")

  add_library(vix_time INTERFACE)
  add_library(vix::time ALIAS vix_time)

  target_compile_features(vix_time INTERFACE cxx_std_20)

  target_include_directories(vix_time INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  )

  # Optional umbrella helpers (if the parent project provides them)
  if (TARGET vix_warnings)
    target_link_libraries(vix_time INTERFACE vix_warnings)
  endif()

  if (VIX_ENABLE_SANITIZERS AND TARGET vix_sanitizers)
    target_link_libraries(vix_time INTERFACE vix_sanitizers)
  endif()

  install(TARGETS vix_time
    EXPORT VixTargets
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  )

  # In standalone builds, install headers too.
  if (NOT (DEFINED VIX_UMBRELLA_BUILD AND VIX_UMBRELLA_BUILD))
    install(DIRECTORY include/
      DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
      FILES_MATCHING PATTERN "*.hpp" PATTERN "*.h"
    )
  endif()

endif()

# ======================================================
# Tests
# ======================================================

option(VIX_TIME_BUILD_TESTS "Build time module tests" OFF)

if (VIX_TIME_BUILD_TESTS)
  if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/CMakeLists.txt")
    include(CTest)
    enable_testing()
    add_subdirectory(tests)
  else()
    message(WARNING "[time/tests] VIX_TIME_BUILD_TESTS=ON but tests/CMakeLists.txt not found, skipping.")
  endif()
endif()

# ======================================================
# Examples
# ======================================================

option(VIX_TIME_BUILD_EXAMPLES "Build time module examples" OFF)

if (VIX_TIME_BUILD_EXAMPLES)
  if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/examples/CMakeLists.txt")
    add_subdirectory(examples)
  else()
    message(WARNING "[time/examples] VIX_TIME_BUILD_EXAMPLES=ON but examples/CMakeLists.txt not found, skipping.")
  endif()
endif()

# ======================================================
# Benchmarks
# ======================================================

option(VIX_TIME_BUILD_BENCH "Build time module benchmarks" OFF)

if (VIX_TIME_BUILD_BENCH)
  if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/bench/CMakeLists.txt")
    add_subdirectory(src/bench)
  else()
    message(WARNING "[time/bench] VIX_TIME_BUILD_BENCH=ON but src/bench/CMakeLists.txt not found, skipping.")
  endif()
endif()

# ======================================================
# Summary
# ======================================================

message(STATUS "------------------------------------------------------")
message(STATUS "vix::time configured (${PROJECT_VERSION})")
if (TIME_SOURCES)
  message(STATUS "Mode: STATIC / sources found")
else()
  message(STATUS "Mode: HEADER-ONLY / no sources")
endif()
message(STATUS "Include dir: ${CMAKE_CURRENT_SOURCE_DIR}/include (for <vix/time/...>)")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Build tests: ${VIX_TIME_BUILD_TESTS}")
message(STATUS "Build examples: ${VIX_TIME_BUILD_EXAMPLES}")
message(STATUS "------------------------------------------------------")
