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
 * Vix.cpp
 *
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
   * @details
   * Use this when you need the current real-world time, such as:
   * - timestamps in logs
   * - persisted records (files, databases, events)
   * - user-facing "created_at" / "updated_at"
   *
   * The returned value is an epoch-based @ref Timestamp (nanoseconds since epoch).
   *
   * @note
   * Wall time can jump (NTP sync, manual clock change, VM suspend/resume).
   * For measuring elapsed time, prefer @ref SteadyClock.
   */
  struct SystemClock
  {
    /**
     * @brief Get the current wall-clock time as a @ref Timestamp.
     *
     * @return Current time (nanoseconds since epoch).
     */
    static Timestamp now() noexcept
    {
      return Timestamp::now();
    }
  };

  /**
   * @brief Steady/monotonic clock helpers.
   *
   * @details
   * Use this for measuring elapsed time reliably:
   * - timeouts
   * - benchmarks
   * - profiling durations
   *
   * This clock is monotonic and does not track wall time.
   * It is not convertible to @ref Timestamp.
   *
   * @par Beginner example
   * @code
   * using vix::time::SteadyClock;
   *
   * const auto start = SteadyClock::now_chrono();
   * // ... work ...
   * const auto elapsed = SteadyClock::since(start);
   * @endcode
   */
  class SteadyClock
  {
  public:
    /// Internal chrono time_point type used by @ref SteadyClock.
    using chrono_tp = std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds>;

    /**
     * @brief Get a monotonic time point with nanosecond precision.
     *
     * @return Current steady_clock time_point cast to nanoseconds.
     */
    static chrono_tp now_chrono() noexcept
    {
      return std::chrono::time_point_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now());
    }

    /**
     * @brief Compute the elapsed duration since @p start.
     *
     * @param start A time point returned by @ref now_chrono.
     * @return Elapsed time as a @ref Duration.
     *
     * @note
     * Passing a time point from a different clock type is undefined behavior.
     */
    static Duration since(const chrono_tp &start) noexcept
    {
      const auto delta = now_chrono() - start;
      return Duration(std::chrono::duration_cast<std::chrono::nanoseconds>(delta));
    }
  };

} // namespace vix::time

#endif // VIX_TIME_CLOCK_HPP
