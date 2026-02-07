/**
 * @file Duration.hpp
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
 * Vix.cpp — Time Module / Duration
 * =====================================================
 *
 * Explicit duration type used across Vix.
 *
 * - Thin wrapper over std::chrono::duration
 * - Strongly typed (no raw integers)
 * - Clear factory helpers (seconds, millis, etc.)
 * - Safe conversions
 *
 * Designed to be predictable, readable, and easy to use.
 */

#ifndef VIX_TIME_DURATION_HPP
#define VIX_TIME_DURATION_HPP

#include <chrono>
#include <cstdint>

namespace vix::time
{
  /**
   * @brief Explicit duration wrapper.
   *
   * Internally stores duration in nanoseconds for maximum precision.
   * Conversion helpers are provided for common units.
   */
  class Duration
  {
  public:
    using rep = std::int64_t;
    using chrono_ns = std::chrono::nanoseconds;

    // --------------------------------------------------
    // Constructors
    // --------------------------------------------------

    constexpr Duration() noexcept : ns_(0) {}

    explicit constexpr Duration(chrono_ns ns) noexcept
        : ns_(ns.count())
    {
    }

    // --------------------------------------------------
    // Factory helpers
    // --------------------------------------------------

    static constexpr Duration nanoseconds(rep ns) noexcept
    {
      return Duration(chrono_ns(ns));
    }

    static constexpr Duration microseconds(rep us) noexcept
    {
      return Duration(std::chrono::microseconds(us));
    }

    static constexpr Duration milliseconds(rep ms) noexcept
    {
      return Duration(std::chrono::milliseconds(ms));
    }

    static constexpr Duration seconds(rep s) noexcept
    {
      return Duration(std::chrono::seconds(s));
    }

    static constexpr Duration minutes(rep m) noexcept
    {
      return Duration(std::chrono::minutes(m));
    }

    static constexpr Duration hours(rep h) noexcept
    {
      return Duration(std::chrono::hours(h));
    }

    // --------------------------------------------------
    // Accessors
    // --------------------------------------------------

    constexpr rep count_ns() const noexcept { return ns_; }

    constexpr rep count_us() const noexcept
    {
      return std::chrono::duration_cast<std::chrono::microseconds>(to_chrono()).count();
    }

    constexpr rep count_ms() const noexcept
    {
      return std::chrono::duration_cast<std::chrono::milliseconds>(to_chrono()).count();
    }

    constexpr rep count_seconds() const noexcept
    {
      return std::chrono::duration_cast<std::chrono::seconds>(to_chrono()).count();
    }

    // --------------------------------------------------
    // Conversions
    // --------------------------------------------------

    constexpr chrono_ns to_chrono() const noexcept
    {
      return chrono_ns(ns_);
    }

    // --------------------------------------------------
    // Comparison
    // --------------------------------------------------

    friend constexpr bool operator==(const Duration &a, const Duration &b) noexcept
    {
      return a.ns_ == b.ns_;
    }

    friend constexpr bool operator!=(const Duration &a, const Duration &b) noexcept
    {
      return !(a == b);
    }

    friend constexpr bool operator<(const Duration &a, const Duration &b) noexcept
    {
      return a.ns_ < b.ns_;
    }

    friend constexpr bool operator>(const Duration &a, const Duration &b) noexcept
    {
      return b < a;
    }

    friend constexpr bool operator<=(const Duration &a, const Duration &b) noexcept
    {
      return !(b < a);
    }

    friend constexpr bool operator>=(const Duration &a, const Duration &b) noexcept
    {
      return !(a < b);
    }

    // --------------------------------------------------
    // Arithmetic
    // --------------------------------------------------

    friend constexpr Duration operator+(const Duration &a, const Duration &b) noexcept
    {
      return Duration::nanoseconds(a.ns_ + b.ns_);
    }

    friend constexpr Duration operator-(const Duration &a, const Duration &b) noexcept
    {
      return Duration::nanoseconds(a.ns_ - b.ns_);
    }

    constexpr Duration &operator+=(const Duration &other) noexcept
    {
      ns_ += other.ns_;
      return *this;
    }

    constexpr Duration &operator-=(const Duration &other) noexcept
    {
      ns_ -= other.ns_;
      return *this;
    }

    // --------------------------------------------------
    // Utilities
    // --------------------------------------------------

    constexpr bool is_zero() const noexcept
    {
      return ns_ == 0;
    }

  private:
    rep ns_;
  };

} // namespace vix::time

#endif // VIX_TIME_DURATION_HPP
