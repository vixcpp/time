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
 * Vix.cpp
 */

#ifndef VIX_TIME_DURATION_HPP
#define VIX_TIME_DURATION_HPP

#include <chrono>
#include <cstdint>

namespace vix::time
{
  /**
   * @brief Explicit duration wrapper with nanosecond precision.
   *
   * @details
   * Duration is a small, strongly typed value used to represent elapsed time
   * across Vix.cpp.
   *
   * Internally, it stores time as a signed 64-bit count of nanoseconds.
   * This ensures:
   * - maximum precision
   * - deterministic arithmetic
   * - no implicit unit confusion
   *
   * The API is intentionally explicit and readable, suitable for beginners,
   * while still mapping directly to std::chrono for advanced use cases.
   */
  class Duration
  {
  public:
    using rep = std::int64_t;
    using chrono_ns = std::chrono::nanoseconds;

    /**
     * @brief Construct a zero duration.
     */
    constexpr Duration() noexcept : ns_(0) {}

    /**
     * @brief Construct from a chrono nanoseconds duration.
     */
    explicit constexpr Duration(chrono_ns ns) noexcept
        : ns_(ns.count())
    {
    }

    /**
     * @brief Create a duration from nanoseconds.
     */
    static constexpr Duration nanoseconds(rep ns) noexcept
    {
      return Duration(chrono_ns(ns));
    }

    /**
     * @brief Create a duration from microseconds.
     */
    static constexpr Duration microseconds(rep us) noexcept
    {
      return Duration(std::chrono::microseconds(us));
    }

    /**
     * @brief Create a duration from milliseconds.
     */
    static constexpr Duration milliseconds(rep ms) noexcept
    {
      return Duration(std::chrono::milliseconds(ms));
    }

    /**
     * @brief Create a duration from seconds.
     */
    static constexpr Duration seconds(rep s) noexcept
    {
      return Duration(std::chrono::seconds(s));
    }

    /**
     * @brief Create a duration from minutes.
     */
    static constexpr Duration minutes(rep m) noexcept
    {
      return Duration(std::chrono::minutes(m));
    }

    /**
     * @brief Create a duration from hours.
     */
    static constexpr Duration hours(rep h) noexcept
    {
      return Duration(std::chrono::hours(h));
    }

    /**
     * @brief Return the duration in nanoseconds.
     */
    constexpr rep count_ns() const noexcept { return ns_; }

    /**
     * @brief Return the duration in microseconds (truncated).
     */
    constexpr rep count_us() const noexcept
    {
      return std::chrono::duration_cast<std::chrono::microseconds>(to_chrono()).count();
    }

    /**
     * @brief Return the duration in milliseconds (truncated).
     */
    constexpr rep count_ms() const noexcept
    {
      return std::chrono::duration_cast<std::chrono::milliseconds>(to_chrono()).count();
    }

    /**
     * @brief Return the duration in seconds (truncated).
     */
    constexpr rep count_seconds() const noexcept
    {
      return std::chrono::duration_cast<std::chrono::seconds>(to_chrono()).count();
    }

    /**
     * @brief Convert to std::chrono::nanoseconds.
     */
    constexpr chrono_ns to_chrono() const noexcept
    {
      return chrono_ns(ns_);
    }

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

    /**
     * @brief Check if this duration is zero.
     */
    constexpr bool is_zero() const noexcept
    {
      return ns_ == 0;
    }

  private:
    rep ns_;
  };

} // namespace vix::time

#endif // VIX_TIME_DURATION_HPP
