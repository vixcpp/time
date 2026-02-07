/**
 * @file Date.hpp
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

#ifndef VIX_TIME_DATE_HPP
#define VIX_TIME_DATE_HPP

#include <chrono>
#include <iomanip>
#include <sstream>
#include <string>
#include <string_view>

#include <vix/time/Parse.hpp>
#include <vix/time/Timestamp.hpp>

namespace vix::time
{
  /**
   * @brief Calendar date (year-month-day), without time and without timezone.
   *
   * @details
   * Date is a small value type representing a calendar day.
   * It does not store timezone data.
   *
   * Converting a Date to a @ref Timestamp is done in UTC at 00:00:00.
   *
   * This type is designed to stay friendly for beginners (simple fields, simple parsing)
   * while remaining useful for advanced users (chrono-based validation and conversion).
   *
   * @par Beginner example
   * @code
   * using vix::time::Date;
   *
   * Date d = Date::parse("2026-02-07");
   * if (!d.is_valid())
   *   return;
   *
   * std::cout << d.to_string() << "\n";
   * @endcode
   */
  class Date
  {
  public:
    /**
     * @brief Construct the default date: 1970-01-01.
     */
    constexpr Date() noexcept
        : year_(1970), month_(1), day_(1)
    {
    }

    /**
     * @brief Construct a date from explicit fields.
     *
     * @param year  Full year (e.g. 2026).
     * @param month Month in [1..12] (range is checked by @ref is_valid).
     * @param day   Day in [1..31] (calendar validation is checked by @ref is_valid).
     *
     * @note
     * This constructor does not validate the calendar day. Use @ref is_valid
     * to check validity.
     */
    constexpr Date(int year, int month, int day) noexcept
        : year_(year), month_(month), day_(day)
    {
    }

    /**
     * @brief Current date in UTC.
     *
     * @return Today's date in UTC.
     *
     * @note
     * This is an alias of @ref today.
     */
    static Date now() noexcept
    {
      return today();
    }

    /**
     * @brief Current date in UTC.
     *
     * @return Today's date in UTC.
     */
    static Date today() noexcept
    {
      using namespace std::chrono;

      const auto tp = floor<days>(system_clock::now());
      const year_month_day ymd{tp};

      return Date(
          int(ymd.year()),
          int(unsigned(ymd.month())),
          int(unsigned(ymd.day())));
    }

    /**
     * @brief Parse a date from "YYYY-MM-DD".
     *
     * @details
     * On parse failure, this returns the default date (1970-01-01).
     *
     * @param s Input string view.
     * @return Parsed date, or default date on failure.
     *
     * @note
     * Parsing succeeds even if the date is not a valid calendar day.
     * Use @ref is_valid to validate.
     */
    static Date parse(std::string_view s) noexcept
    {
      int y = 0;
      int m = 0;
      int d = 0;

      if (!vix::time::parse::parse_ymd(s, y, m, d))
        return Date();

      return Date(y, m, d);
    }

    /**
     * @brief Get the year component.
     */
    constexpr int year() const noexcept { return year_; }

    /**
     * @brief Get the month component (as stored).
     *
     * @return Month in [1..12] if valid.
     */
    constexpr int month() const noexcept { return month_; }

    /**
     * @brief Get the day component (as stored).
     *
     * @return Day in [1..31] if valid.
     */
    constexpr int day() const noexcept { return day_; }

    /**
     * @brief Check if this date is a valid calendar day.
     *
     * @return True if year-month-day forms a valid date, false otherwise.
     */
    constexpr bool is_valid() const noexcept
    {
      using namespace std::chrono;

      const year_month_day ymd{
          std::chrono::year{year_},
          std::chrono::month{static_cast<unsigned>(month_)},
          std::chrono::day{static_cast<unsigned>(day_)}};

      return ymd.ok();
    }

    /**
     * @brief Convert this date to a UTC timestamp at 00:00:00.
     *
     * @details
     * If the date is invalid (@ref is_valid is false), this returns a default
     * constructed @ref Timestamp.
     *
     * @return UTC timestamp at midnight for this date, or default Timestamp on invalid date.
     */
    Timestamp to_timestamp_utc() const noexcept
    {
      using namespace std::chrono;

      const year_month_day ymd{
          std::chrono::year{year_},
          std::chrono::month{static_cast<unsigned>(month_)},
          std::chrono::day{static_cast<unsigned>(day_)}};

      if (!ymd.ok())
        return Timestamp();

      const sys_days day_tp{ymd};
      const sys_time<nanoseconds> tp = time_point_cast<nanoseconds>(day_tp);

      return Timestamp(tp);
    }

    /**
     * @brief Format as "YYYY-MM-DD".
     *
     * @return String representation of the date.
     *
     * @note
     * This does not validate the date. It prints stored fields.
     */
    std::string to_string() const
    {
      std::ostringstream oss;
      oss << std::setfill('0')
          << std::setw(4) << year_ << "-"
          << std::setw(2) << month_ << "-"
          << std::setw(2) << day_;
      return oss.str();
    }

    /**
     * @brief Equality compare.
     */
    friend constexpr bool operator==(const Date &a, const Date &b) noexcept
    {
      return a.year_ == b.year_ && a.month_ == b.month_ && a.day_ == b.day_;
    }

    /**
     * @brief Inequality compare.
     */
    friend constexpr bool operator!=(const Date &a, const Date &b) noexcept
    {
      return !(a == b);
    }

    /**
     * @brief Strict weak ordering (lexicographic by year, month, day).
     */
    friend constexpr bool operator<(const Date &a, const Date &b) noexcept
    {
      if (a.year_ != b.year_)
        return a.year_ < b.year_;
      if (a.month_ != b.month_)
        return a.month_ < b.month_;
      return a.day_ < b.day_;
    }

    /**
     * @brief Greater-than.
     */
    friend constexpr bool operator>(const Date &a, const Date &b) noexcept
    {
      return b < a;
    }

    /**
     * @brief Less-or-equal.
     */
    friend constexpr bool operator<=(const Date &a, const Date &b) noexcept
    {
      return !(b < a);
    }

    /**
     * @brief Greater-or-equal.
     */
    friend constexpr bool operator>=(const Date &a, const Date &b) noexcept
    {
      return !(a < b);
    }

  private:
    int year_;
    int month_;
    int day_;
  };

} // namespace vix::time

#endif // VIX_TIME_DATE_HPP
