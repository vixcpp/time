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
 * Vix.cpp
 */

#ifndef VIX_TIME_TIMESTAMP_HPP
#define VIX_TIME_TIMESTAMP_HPP

#include <chrono>
#include <cstdint>

#include <vix/time/Duration.hpp>

namespace vix::time
{
  /**
   * @brief Absolute point in time (epoch-based).
   *
   * @details
   * Timestamp represents an absolute moment in time as a signed number of
   * nanoseconds since the Unix epoch:
   *
   * 1970-01-01T00:00:00Z
   *
   * It is designed as a thin, explicit wrapper over
   * `std::chrono::time_point`, with a stable integer representation.
   *
   * Typical use cases include:
   * - ordering and comparisons
   * - persistence (WAL, logs, caches)
   * - synchronization and replication
   * - time arithmetic with @ref Duration
   *
   * The API favors predictability and durability over convenience magic.
   */
  class Timestamp
  {
  public:
    /// Signed integer type used to store nanoseconds since epoch.
    using rep = std::int64_t;

    /// Chrono representation used for interop with std::chrono.
    using chrono_tp =
        std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds>;

    /**
     * @brief Construct a zero timestamp (epoch).
     *
     * This represents 1970-01-01T00:00:00Z.
     */
    constexpr Timestamp() noexcept : ns_since_epoch_(0) {}

    /**
     * @brief Construct from nanoseconds since epoch.
     *
     * @param ns_since_epoch Nanoseconds since Unix epoch.
     */
    explicit constexpr Timestamp(rep ns_since_epoch) noexcept
        : ns_since_epoch_(ns_since_epoch)
    {
    }

    /**
     * @brief Construct from a chrono time_point.
     *
     * @param tp Time point expressed in nanoseconds.
     */
    explicit constexpr Timestamp(chrono_tp tp) noexcept
        : ns_since_epoch_(tp.time_since_epoch().count())
    {
    }

    /**
     * @brief Get the current time as a UTC timestamp.
     *
     * @return Current timestamp (nanoseconds since epoch).
     */
    static Timestamp now() noexcept
    {
      const auto tp =
          std::chrono::time_point_cast<std::chrono::nanoseconds>(
              std::chrono::system_clock::now());
      return Timestamp(tp);
    }

    /**
     * @brief Build a timestamp from nanoseconds since epoch.
     */
    static constexpr Timestamp from_nanoseconds(rep ns) noexcept
    {
      return Timestamp(ns);
    }

    /**
     * @brief Build a timestamp from seconds since epoch.
     */
    static constexpr Timestamp from_seconds(rep sec) noexcept
    {
      return Timestamp(sec * 1'000'000'000LL);
    }

    /**
     * @brief Return nanoseconds since epoch.
     */
    constexpr rep nanoseconds_since_epoch() const noexcept
    {
      return ns_since_epoch_;
    }

    /**
     * @brief Return seconds since epoch (truncated).
     */
    constexpr rep seconds_since_epoch() const noexcept
    {
      return ns_since_epoch_ / 1'000'000'000LL;
    }

    /**
     * @brief Convert to std::chrono::time_point.
     */
    constexpr chrono_tp to_chrono() const noexcept
    {
      return chrono_tp(std::chrono::nanoseconds(ns_since_epoch_));
    }

    /**
     * @brief Add a duration to this timestamp.
     */
    friend constexpr Timestamp operator+(const Timestamp &ts,
                                         const Duration &d) noexcept
    {
      return Timestamp::from_nanoseconds(
          ts.ns_since_epoch_ + d.count_ns());
    }

    /**
     * @brief Subtract a duration from this timestamp.
     */
    friend constexpr Timestamp operator-(const Timestamp &ts,
                                         const Duration &d) noexcept
    {
      return Timestamp::from_nanoseconds(
          ts.ns_since_epoch_ - d.count_ns());
    }

    /**
     * @brief Compute the duration between two timestamps.
     */
    friend constexpr Duration operator-(const Timestamp &a,
                                        const Timestamp &b) noexcept
    {
      return Duration::nanoseconds(
          a.ns_since_epoch_ - b.ns_since_epoch_);
    }

    /**
     * @brief Equality compare.
     */
    friend constexpr bool operator==(const Timestamp &a,
                                     const Timestamp &b) noexcept
    {
      return a.ns_since_epoch_ == b.ns_since_epoch_;
    }

    /**
     * @brief Inequality compare.
     */
    friend constexpr bool operator!=(const Timestamp &a,
                                     const Timestamp &b) noexcept
    {
      return !(a == b);
    }

    /**
     * @brief Strict weak ordering.
     */
    friend constexpr bool operator<(const Timestamp &a,
                                    const Timestamp &b) noexcept
    {
      return a.ns_since_epoch_ < b.ns_since_epoch_;
    }

    /**
     * @brief Greater-than.
     */
    friend constexpr bool operator>(const Timestamp &a,
                                    const Timestamp &b) noexcept
    {
      return b < a;
    }

    /**
     * @brief Less-or-equal.
     */
    friend constexpr bool operator<=(const Timestamp &a,
                                     const Timestamp &b) noexcept
    {
      return !(b < a);
    }

    /**
     * @brief Greater-or-equal.
     */
    friend constexpr bool operator>=(const Timestamp &a,
                                     const Timestamp &b) noexcept
    {
      return !(a < b);
    }

    /**
     * @brief Check if this timestamp is the epoch (zero).
     */
    constexpr bool is_zero() const noexcept
    {
      return ns_since_epoch_ == 0;
    }

  private:
    rep ns_since_epoch_;
  };

} // namespace vix::time

#endif // VIX_TIME_TIMESTAMP_HPP
