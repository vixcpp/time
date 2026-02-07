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
# Vix.cpp - vix::time examples
# ====================================================================

cmake_minimum_required(VERSION 3.20)

# Collect example sources (each .cpp becomes one executable)
file(GLOB EXAMPLE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

if (NOT EXAMPLE_SOURCES)
  message(STATUS "[time/examples] No .cpp examples found.")
  return()
endif()

message(STATUS "[time/examples] Found ${EXAMPLE_SOURCES}")

foreach (src ${EXAMPLE_SOURCES})
  get_filename_component(name ${src} NAME_WE)

  # Prefix to avoid collisions in umbrella builds
  set(target "vix_time_example_${name}")

  add_executable(${target} ${src})

  target_compile_features(${target} PRIVATE cxx_std_20)

  # Link the module
  target_link_libraries(${target} PRIVATE vix::time)

  # Optional umbrella helpers (if present)
  if (TARGET vix_warnings)
    target_link_libraries(${target} PRIVATE vix_warnings)
  endif()

  if (VIX_ENABLE_SANITIZERS AND TARGET vix_sanitizers)
    target_link_libraries(${target} PRIVATE vix_sanitizers)
  endif()

  # Nice output folder in IDEs
  set_target_properties(${target} PROPERTIES
    FOLDER "examples/time"
    OUTPUT_NAME ${name}
  )

  message(STATUS "[time/examples] Added ${target} -> ${name}")
endforeach()
