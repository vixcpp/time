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
# ======================================================
# Vix.cpp — Time Module Tests
# ======================================================

cmake_minimum_required(VERSION 3.20)

# If this tests folder is included from the module root, the parent
# already defined the project() and enabled C++20.
# We only declare tests here.

file(GLOB TIME_TEST_SOURCES
  "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

if (NOT TIME_TEST_SOURCES)
  message(STATUS "[time/tests] No tests found.")
  return()
endif()

# One binary per test file (simple and explicit)
foreach(test_src ${TIME_TEST_SOURCES})
  get_filename_component(test_name ${test_src} NAME_WE)

  add_executable(${test_name} ${test_src})

  target_compile_features(${test_name} PRIVATE cxx_std_20)

  target_link_libraries(${test_name}
    PRIVATE
      vix::time
  )

  add_test(NAME time.${test_name} COMMAND ${test_name})
endforeach()

message(STATUS "[time/tests] Configured ${TIME_TEST_SOURCES}")
