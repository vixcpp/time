/**
 * @file DateTime.hpp
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

#ifndef VIX_TIME_DATE_TIME_HPP
#define VIX_TIME_DATE_TIME_HPP

#include <chrono>
#include <cstdint>
#include <iomanip>
#include <sstream>
#include <string>
#include <string_view>

#include <vix/time/Timestamp.hpp>

namespace vix::time
{
  /**
   * @brief Explicit calendar date + time (UTC-oriented), with nanosecond precision.
   *
   * @details
   * DateTime is a lightweight value type representing:
   * year-month-day hour:minute:second.nanosecond
   *
   * It does not store a timezone. In v1, parsing and conversions are treated as UTC:
   * - @ref to_timestamp_utc converts this DateTime to a @ref Timestamp assuming UTC
   * - @ref from_timestamp_utc builds a DateTime from an epoch timestamp in UTC
   *
   * This keeps behavior simple and predictable for beginners, while still enabling
   * advanced users to build higher-level timezone features on top.
   *
   * @par Supported parse forms (v1)
   * - "YYYY-MM-DDTHH:MM:SSZ"
   * - "YYYY-MM-DD HH:MM:SS" (treated as UTC)
   * - Optional fractional seconds after '.':
   *   - "YYYY-MM-DDTHH:MM:SS.sssZ"
   *   - "YYYY-MM-DDTHH:MM:SS.ssssssZ"
   *   - "YYYY-MM-DDTHH:MM:SS.sssssssssZ"
   *
   * @par Beginner example
   * @code
   * using vix::time::DateTime;
   *
   * auto dt = DateTime::parse("2026-02-07T12:30:05.123Z");
   * std::cout << dt.to_string_utc() << "\n";
   * @endcode
   */
  class DateTime
  {
  public:
    /// Storage type for nanoseconds field.
    using nanos_rep = std::int32_t;

    /**
     * @brief Construct the default datetime: 1970-01-01T00:00:00Z.
     */
    constexpr DateTime() noexcept
        : year_(1970), month_(1), day_(1),
          hour_(0), minute_(0), second_(0),
          nanosecond_(0)
    {
    }

    /**
     * @brief Construct a DateTime from explicit fields.
     *
     * @param year Full year (e.g. 2026).
     * @param month Month in [1..12] (strict calendar validation is done by conversion).
     * @param day Day in [1..31] (strict calendar validation is done by conversion).
     * @param hour Hour in [0..23].
     * @param minute Minute in [0..59].
     * @param second Second in [0..60] (60 allowed for leap second style inputs).
     * @param nanosecond Nanoseconds in [0..999999999].
     *
     * @note
     * This constructor does not fully validate the date. Conversions that require
     * a valid calendar day will validate (see @ref to_timestamp_utc).
     */
    constexpr DateTime(int year, int month, int day,
                       int hour, int minute, int second,
                       nanos_rep nanosecond = 0) noexcept
        : year_(year), month_(month), day_(day),
          hour_(hour), minute_(minute), second_(second),
          nanosecond_(nanosecond)
    {
    }

    /**
     * @brief Current datetime in UTC.
     *
     * @return Current UTC datetime derived from @ref Timestamp::now.
     */
    static DateTime now_utc() noexcept
    {
      return from_timestamp_utc(Timestamp::now());
    }

    /**
     * @brief Build a UTC DateTime from an epoch-based timestamp.
     *
     * @param ts Epoch-based timestamp.
     * @return UTC DateTime view of @p ts.
     */
    static DateTime from_timestamp_utc(const Timestamp &ts) noexcept
    {
      using namespace std::chrono;

      const auto tp = ts.to_chrono();

      const auto day_tp = floor<days>(tp);
      const year_month_day ymd{day_tp};

      const auto tod = hh_mm_ss<nanoseconds>(tp - day_tp);

      return DateTime(
          int(ymd.year()),
          int(unsigned(ymd.month())),
          int(unsigned(ymd.day())),
          int(tod.hours().count()),
          int(tod.minutes().count()),
          int(tod.seconds().count()),
          nanos_rep(tod.subseconds().count()));
    }

    /**
     * @brief Parse a basic ISO-8601 datetime string (UTC in v1).
     *
     * @details
     * Accepts:
     * - YYYY-MM-DDTHH:MM:SSZ
     * - YYYY-MM-DD HH:MM:SS
     * - Optional fractional seconds after '.'
     * - Optional trailing 'Z'
     *
     * On parse failure, returns the default value (1970-01-01T00:00:00Z).
     *
     * @param s Input string view.
     * @return Parsed DateTime, or default value on failure.
     *
     * @note
     * This parser performs basic range checks. Full calendar validity is enforced
     * when converting via @ref to_timestamp_utc.
     */
    static DateTime parse(std::string_view s)
    {
      int y = 0, mo = 0, d = 0;
      int h = 0, mi = 0, se = 0;

      std::int64_t frac = 0;
      int frac_digits = 0;

      char c1 = 0, c2 = 0, c3 = 0, c4 = 0;
      char sep_date_time = 0;

      std::string str(s);
      std::istringstream iss(str);

      // YYYY-MM-DD{T| }HH:MM:SS
      iss >> y >> c1 >> mo >> c2 >> d >> sep_date_time >> h >> c3 >> mi >> c4 >> se;

      if (!iss || c1 != '-' || c2 != '-' || c3 != ':' || c4 != ':' ||
          (sep_date_time != 'T' && sep_date_time != ' '))
      {
        return DateTime();
      }

      // Optional fractional part: .ssss...
      if (iss.peek() == '.')
      {
        iss.get(); // '.'
        while (true)
        {
          const int ch = iss.peek();
          if (ch < '0' || ch > '9')
            break;

          iss.get();
          if (frac_digits < 9)
          {
            frac = (frac * 10) + (ch - '0');
            ++frac_digits;
          }
          else
          {
            // Ignore extra digits beyond nanoseconds precision.
            ++frac_digits;
          }
        }
      }

      // Optional trailing Z
      if (iss.peek() == 'Z')
        iss.get();

      // Normalize fractional seconds to nanoseconds (pad with zeros).
      while (frac_digits > 0 && frac_digits < 9)
      {
        frac *= 10;
        ++frac_digits;
      }

      // Basic range sanity (calendar validity is checked by to_timestamp_utc).
      if (mo < 1 || mo > 12 || d < 1 || d > 31 ||
          h < 0 || h > 23 || mi < 0 || mi > 59 || se < 0 || se > 60)
      {
        return DateTime();
      }

      if (frac < 0 || frac > 999999999)
        return DateTime();

      return DateTime(y, mo, d, h, mi, se, nanos_rep(frac));
    }

    /// Get year component.
    constexpr int year() const noexcept { return year_; }
    /// Get month component (as stored).
    constexpr int month() const noexcept { return month_; }
    /// Get day component (as stored).
    constexpr int day() const noexcept { return day_; }

    /// Get hour component.
    constexpr int hour() const noexcept { return hour_; }
    /// Get minute component.
    constexpr int minute() const noexcept { return minute_; }
    /// Get second component.
    constexpr int second() const noexcept { return second_; }
    /// Get nanosecond component.
    constexpr nanos_rep nanosecond() const noexcept { return nanosecond_; }

    /**
     * @brief Convert to an epoch-based timestamp (UTC).
     *
     * @details
     * This validates the calendar date (year-month-day). If invalid, returns a
     * default constructed @ref Timestamp.
     *
     * @return UTC timestamp for this DateTime, or default Timestamp on invalid date.
     *
     * @note
     * This assumes the stored fields represent UTC (v1 behavior).
     */
    Timestamp to_timestamp_utc() const noexcept
    {
      using namespace std::chrono;

      const std::chrono::year_month_day ymd{
          std::chrono::year{year_},
          std::chrono::month{static_cast<unsigned>(month_)},
          std::chrono::day{static_cast<unsigned>(day_)}};

      if (!ymd.ok())
        return Timestamp();

      const sys_days day_tp{ymd};

      const auto tp =
          time_point_cast<nanoseconds>(day_tp) +
          hours(hour_) +
          minutes(minute_) +
          seconds(second_) +
          nanoseconds(nanosecond_);

      return Timestamp(tp);
    }

    /**
     * @brief Format as "YYYY-MM-DDTHH:MM:SSZ" (UTC).
     *
     * @details
     * Fractional seconds are omitted when nanosecond is 0.
     *
     * @return ISO-like UTC string.
     *
     * @note
     * Formatting prints stored fields and does not validate the date.
     */
    std::string to_string_utc() const
    {
      std::ostringstream oss;
      oss << std::setfill('0')
          << std::setw(4) << year_ << "-"
          << std::setw(2) << month_ << "-"
          << std::setw(2) << day_ << "T"
          << std::setw(2) << hour_ << ":"
          << std::setw(2) << minute_ << ":"
          << std::setw(2) << second_;

      if (nanosecond_ != 0)
        oss << "." << std::setw(9) << nanosecond_;

      oss << "Z";
      return oss.str();
    }

    /**
     * @brief Equality compare (field-wise).
     */
    friend constexpr bool operator==(const DateTime &a, const DateTime &b) noexcept
    {
      return a.year_ == b.year_ &&
             a.month_ == b.month_ &&
             a.day_ == b.day_ &&
             a.hour_ == b.hour_ &&
             a.minute_ == b.minute_ &&
             a.second_ == b.second_ &&
             a.nanosecond_ == b.nanosecond_;
    }

    /**
     * @brief Inequality compare.
     */
    friend constexpr bool operator!=(const DateTime &a, const DateTime &b) noexcept
    {
      return !(a == b);
    }

    /**
     * @brief Strict weak ordering based on UTC timestamp conversion.
     *
     * @note
     * If either value is invalid, it converts to default Timestamp and ordering
     * follows that result.
     */
    friend bool operator<(const DateTime &a, const DateTime &b) noexcept
    {
      return a.to_timestamp_utc() < b.to_timestamp_utc();
    }

    /// Greater-than.
    friend bool operator>(const DateTime &a, const DateTime &b) noexcept { return b < a; }
    /// Less-or-equal.
    friend bool operator<=(const DateTime &a, const DateTime &b) noexcept { return !(b < a); }
    /// Greater-or-equal.
    friend bool operator>=(const DateTime &a, const DateTime &b) noexcept { return !(a < b); }

  private:
    int year_;
    int month_;
    int day_;

    int hour_;
    int minute_;
    int second_;

    nanos_rep nanosecond_;
  };

} // namespace vix::time

#endif // VIX_TIME_DATE_TIME_HPP
