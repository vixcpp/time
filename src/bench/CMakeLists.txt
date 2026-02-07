cmake_minimum_required(VERSION 3.20)

add_executable(vix_time_bench bench_time.cpp)

target_compile_features(vix_time_bench PRIVATE cxx_std_20)

target_link_libraries(vix_time_bench PRIVATE vix::time)

if (TARGET vix_warnings)
  target_link_libraries(vix_time_bench PRIVATE vix_warnings)
endif()

if (VIX_ENABLE_SANITIZERS AND TARGET vix_sanitizers)
  target_link_libraries(vix_time_bench PRIVATE vix_sanitizers)
endif()

set_target_properties(vix_time_bench PROPERTIES
  FOLDER "bench/time"
)
