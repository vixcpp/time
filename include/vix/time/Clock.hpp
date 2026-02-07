/**
 * @file Clock.hpp
 * @author Gaspard Kirira
 *
 * Copyright 2026, Gaspard Kirira.
 * All rights reserved.
 * https://github.com/vixcpp/time
 *
 * Use of this source code is governed by a MIT license
 * that can be found in the LICENSE file.
 *
 * =====================================================
 * Vix.cpp — Time Module / Clock
 * =====================================================
 *
 * Clock utilities for retrieving current time.
 *
 * - System clock: wall time, epoch-based (Timestamp)
 * - Steady clock: monotonic time for measuring durations
 *
 * This module keeps APIs explicit and predictable.
 */

#ifndef VIX_TIME_CLOCK_HPP
#define VIX_TIME_CLOCK_HPP

#include <chrono>

#include <vix/time/Duration.hpp>
#include <vix/time/Timestamp.hpp>

namespace vix::time
{
  /**
   * @brief System clock helpers (wall time).
   *
   * Returns epoch-based timestamps (nanoseconds since epoch).
   */
  struct SystemClock
  {
    static Timestamp now() noexcept
    {
      return Timestamp::now();
    }
  };

  /**
   * @brief Steady/monotonic clock helpers.
   *
   * Used for measuring elapsed time (timeouts, benchmarks).
   * Not related to wall time and not convertible to Timestamp.
   */
  class SteadyClock
  {
  public:
    using chrono_tp = std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds>;

    static chrono_tp now_chrono() noexcept
    {
      return std::chrono::time_point_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now());
    }

    static Duration since(const chrono_tp &start) noexcept
    {
      const auto delta = now_chrono() - start;
      return Duration(std::chrono::duration_cast<std::chrono::nanoseconds>(delta));
    }
  };

} // namespace vix::time

#endif // VIX_TIME_CLOCK_HPP
