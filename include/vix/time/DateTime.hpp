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
 * =====================================================
 * Vix.cpp - Time Module / DateTime
 * =====================================================
 *
 * Explicit date + time representation.
 *
 * - Simple, predictable fields (Y-M-D h:m:s.ns)
 * - Easy construction and formatting
 * - UTC-based conversions via Timestamp
 * - Parsing for basic ISO-8601 forms
 *
 * Supported parse forms (v1):
 * - "YYYY-MM-DDTHH:MM:SSZ"
 * - "YYYY-MM-DD HH:MM:SS"        (treated as UTC in v1)
 * - Optional fractional seconds:
 *     "YYYY-MM-DDTHH:MM:SS.sssZ"
 *     "YYYY-MM-DDTHH:MM:SS.ssssssZ"
 *     "YYYY-MM-DDTHH:MM:SS.sssssssssZ"
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
  class DateTime
  {
  public:
    using nanos_rep = std::int32_t;

    // --------------------------------------------------
    // Constructors
    // --------------------------------------------------

    constexpr DateTime() noexcept
        : year_(1970), month_(1), day_(1),
          hour_(0), minute_(0), second_(0),
          nanosecond_(0)
    {
    }

    constexpr DateTime(int year, int month, int day,
                       int hour, int minute, int second,
                       nanos_rep nanosecond = 0) noexcept
        : year_(year), month_(month), day_(day),
          hour_(hour), minute_(minute), second_(second),
          nanosecond_(nanosecond)
    {
    }

    // --------------------------------------------------
    // Factory helpers
    // --------------------------------------------------

    /**
     * @brief Current datetime in UTC.
     */
    static DateTime now_utc() noexcept
    {
      return from_timestamp_utc(Timestamp::now());
    }

    /**
     * @brief Build from epoch-based timestamp (UTC).
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
          unsigned(ymd.month()),
          unsigned(ymd.day()),
          int(tod.hours().count()),
          int(tod.minutes().count()),
          int(tod.seconds().count()),
          nanos_rep(tod.subseconds().count()));
    }

    /**
     * @brief Parse basic ISO-8601 strings (UTC in v1).
     *
     * Returns a default value on parse failure.
     */
    static DateTime parse(std::string_view s)
    {
      // Accept:
      //  - YYYY-MM-DDTHH:MM:SSZ
      //  - YYYY-MM-DD HH:MM:SS
      //  - optional fractional seconds after '.'
      //  - optional trailing 'Z'
      int y = 0, mo = 0, d = 0;
      int h = 0, mi = 0, se = 0;
      std::int64_t frac = 0;
      int frac_digits = 0;

      char c1 = 0, c2 = 0, c3 = 0, c4 = 0, c5 = 0;
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
            // ignore extra digits beyond nanoseconds precision
            ++frac_digits;
          }
        }
      }

      // Optional trailing Z
      if (iss.peek() == 'Z')
      {
        iss.get();
      }

      // Normalize fractional to nanoseconds (pad with zeros).
      while (frac_digits > 0 && frac_digits < 9)
      {
        frac *= 10;
        ++frac_digits;
      }

      // If there were no fractional digits, frac is already 0.
      // Basic range sanity (not strict validation yet).
      if (mo < 1 || mo > 12 || d < 1 || d > 31 ||
          h < 0 || h > 23 || mi < 0 || mi > 59 || se < 0 || se > 60)
      {
        return DateTime();
      }

      return DateTime(y, mo, d, h, mi, se, nanos_rep(frac));
    }

    // --------------------------------------------------
    // Accessors
    // --------------------------------------------------

    constexpr int year() const noexcept { return year_; }
    constexpr int month() const noexcept { return month_; }
    constexpr int day() const noexcept { return day_; }

    constexpr int hour() const noexcept { return hour_; }
    constexpr int minute() const noexcept { return minute_; }
    constexpr int second() const noexcept { return second_; }
    constexpr nanos_rep nanosecond() const noexcept { return nanosecond_; }

    // --------------------------------------------------
    // Conversion
    // --------------------------------------------------

    /**
     * @brief Convert to epoch-based timestamp (UTC).
     *
     * Returns Timestamp() if date is invalid.
     */
    Timestamp to_timestamp_utc() const noexcept
    {
      using namespace std::chrono;

      const std::chrono::year_month_day ymd{
          std::chrono::year{year_},
          std::chrono::month{static_cast<unsigned>(month_)},
          std::chrono::day{static_cast<unsigned>(day_)}};

      if (!ymd.ok())
      {
        return Timestamp();
      }

      const sys_days day_tp{ymd};

      const auto tp =
          time_point_cast<nanoseconds>(day_tp) + hours(hour_) + minutes(minute_) + seconds(second_) + nanoseconds(nanosecond_);

      return Timestamp(tp);
    }

    // --------------------------------------------------
    // Formatting
    // --------------------------------------------------

    /**
     * @brief Format as "YYYY-MM-DDTHH:MM:SSZ" (UTC).
     *
     * Fractional seconds are omitted when nanosecond == 0.
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
      {
        oss << "." << std::setw(9) << nanosecond_;
      }

      oss << "Z";
      return oss.str();
    }

    // --------------------------------------------------
    // Comparison
    // --------------------------------------------------

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

    friend constexpr bool operator!=(const DateTime &a, const DateTime &b) noexcept
    {
      return !(a == b);
    }

    friend bool operator<(const DateTime &a, const DateTime &b) noexcept
    {
      return a.to_timestamp_utc() < b.to_timestamp_utc();
    }

    friend bool operator>(const DateTime &a, const DateTime &b) noexcept
    {
      return b < a;
    }

    friend bool operator<=(const DateTime &a, const DateTime &b) noexcept
    {
      return !(b < a);
    }

    friend bool operator>=(const DateTime &a, const DateTime &b) noexcept
    {
      return !(a < b);
    }

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
