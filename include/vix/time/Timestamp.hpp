/**
 * @file Timestamp.hpp
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
 * Vix.cpp — Time Module / Timestamp
 * =====================================================
 *
 * Absolute point in time represented as nanoseconds since
 * Unix epoch (1970-01-01T00:00:00Z).
 *
 * - Explicit, epoch-based
 * - Thin wrapper over std::chrono::time_point
 * - Designed for durability, sync, WAL, cache, ordering
 */

#ifndef VIX_TIME_TIMESTAMP_HPP
#define VIX_TIME_TIMESTAMP_HPP

#include <chrono>
#include <cstdint>

#include <vix/time/Duration.hpp>

namespace vix::time
{
  /**
   * @brief Absolute timestamp (epoch-based).
   */
  class Timestamp
  {
  public:
    using rep = std::int64_t;
    using chrono_tp = std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds>;

    // --------------------------------------------------
    // Constructors
    // --------------------------------------------------

    constexpr Timestamp() noexcept : ns_since_epoch_(0) {}

    explicit constexpr Timestamp(rep ns_since_epoch) noexcept
        : ns_since_epoch_(ns_since_epoch)
    {
    }

    explicit constexpr Timestamp(chrono_tp tp) noexcept
        : ns_since_epoch_(tp.time_since_epoch().count())
    {
    }

    // --------------------------------------------------
    // Factory helpers
    // --------------------------------------------------

    static Timestamp now() noexcept
    {
      const auto tp = std::chrono::time_point_cast<std::chrono::nanoseconds>(
          std::chrono::system_clock::now());
      return Timestamp(tp);
    }

    static constexpr Timestamp from_nanoseconds(rep ns) noexcept
    {
      return Timestamp(ns);
    }

    static constexpr Timestamp from_seconds(rep sec) noexcept
    {
      return Timestamp(sec * 1'000'000'000LL);
    }

    // --------------------------------------------------
    // Accessors
    // --------------------------------------------------

    constexpr rep nanoseconds_since_epoch() const noexcept
    {
      return ns_since_epoch_;
    }

    constexpr rep seconds_since_epoch() const noexcept
    {
      return ns_since_epoch_ / 1'000'000'000LL;
    }

    // --------------------------------------------------
    // Conversions
    // --------------------------------------------------

    constexpr chrono_tp to_chrono() const noexcept
    {
      return chrono_tp(std::chrono::nanoseconds(ns_since_epoch_));
    }

    // --------------------------------------------------
    // Arithmetic
    // --------------------------------------------------

    friend constexpr Timestamp operator+(const Timestamp &ts, const Duration &d) noexcept
    {
      return Timestamp::from_nanoseconds(ts.ns_since_epoch_ + d.count_ns());
    }

    friend constexpr Timestamp operator-(const Timestamp &ts, const Duration &d) noexcept
    {
      return Timestamp::from_nanoseconds(ts.ns_since_epoch_ - d.count_ns());
    }

    friend constexpr Duration operator-(const Timestamp &a, const Timestamp &b) noexcept
    {
      return Duration::nanoseconds(a.ns_since_epoch_ - b.ns_since_epoch_);
    }

    // --------------------------------------------------
    // Comparison
    // --------------------------------------------------

    friend constexpr bool operator==(const Timestamp &a, const Timestamp &b) noexcept
    {
      return a.ns_since_epoch_ == b.ns_since_epoch_;
    }

    friend constexpr bool operator!=(const Timestamp &a, const Timestamp &b) noexcept
    {
      return !(a == b);
    }

    friend constexpr bool operator<(const Timestamp &a, const Timestamp &b) noexcept
    {
      return a.ns_since_epoch_ < b.ns_since_epoch_;
    }

    friend constexpr bool operator>(const Timestamp &a, const Timestamp &b) noexcept
    {
      return b < a;
    }

    friend constexpr bool operator<=(const Timestamp &a, const Timestamp &b) noexcept
    {
      return !(b < a);
    }

    friend constexpr bool operator>=(const Timestamp &a, const Timestamp &b) noexcept
    {
      return !(a < b);
    }

    // --------------------------------------------------
    // Utilities
    // --------------------------------------------------

    constexpr bool is_zero() const noexcept
    {
      return ns_since_epoch_ == 0;
    }

  private:
    rep ns_since_epoch_;
  };

} // namespace vix::time

#endif // VIX_TIME_TIMESTAMP_HPP
