/**
 * @file Parse.hpp
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

#ifndef VIX_TIME_PARSE_HPP
#define VIX_TIME_PARSE_HPP

#include <cstdint>
#include <string_view>

namespace vix::time::parse
{
  /**
   * @brief Parse a calendar date in the form "YYYY-MM-DD".
   *
   * @details
   * This is a small, dependency-free helper used by higher-level types
   * such as Date and DateTime.
   *
   * It parses digits and separators only. It does not validate calendar
   * correctness (for example 2026-02-31). Calendar validation is done by
   * higher-level types when needed.
   *
   * @param s Input string.
   * @param year Output year.
   * @param month Output month.
   * @param day Output day.
   * @return true on success, false on failure.
   */
  inline bool parse_ymd(std::string_view s,
                        int &year,
                        int &month,
                        int &day) noexcept
  {
    if (s.size() != 10)
      return false;

    // YYYY-MM-DD
    if (s[4] != '-' || s[7] != '-')
      return false;

    auto digit = [](char c) -> int
    {
      return (c >= '0' && c <= '9') ? (c - '0') : -1;
    };

    int y = 0;
    for (int i = 0; i < 4; ++i)
    {
      const int d = digit(s[i]);
      if (d < 0)
        return false;
      y = (y * 10) + d;
    }

    int m = 0;
    for (int i = 5; i < 7; ++i)
    {
      const int d = digit(s[i]);
      if (d < 0)
        return false;
      m = (m * 10) + d;
    }

    int dday = 0;
    for (int i = 8; i < 10; ++i)
    {
      const int d = digit(s[i]);
      if (d < 0)
        return false;
      dday = (dday * 10) + d;
    }

    year = y;
    month = m;
    day = dday;
    return true;
  }

  /**
   * @brief Parse a time in the form "HH:MM:SS".
   *
   * @details
   * This helper parses digits and separators only. It does not validate
   * ranges (such as hour <= 23). Range validation is performed by the
   * higher-level types that use this function.
   *
   * @param s Input string.
   * @param hour Output hour.
   * @param minute Output minute.
   * @param second Output second.
   * @return true on success, false on failure.
   */
  inline bool parse_hms(std::string_view s,
                        int &hour,
                        int &minute,
                        int &second) noexcept
  {
    if (s.size() != 8)
      return false;

    // HH:MM:SS
    if (s[2] != ':' || s[5] != ':')
      return false;

    auto digit = [](char c) -> int
    {
      return (c >= '0' && c <= '9') ? (c - '0') : -1;
    };

    const int h1 = digit(s[0]);
    const int h2 = digit(s[1]);
    const int m1 = digit(s[3]);
    const int m2 = digit(s[4]);
    const int s1 = digit(s[6]);
    const int s2 = digit(s[7]);

    if (h1 < 0 || h2 < 0 || m1 < 0 || m2 < 0 || s1 < 0 || s2 < 0)
      return false;

    hour = (h1 * 10) + h2;
    minute = (m1 * 10) + m2;
    second = (s1 * 10) + s2;
    return true;
  }

} // namespace vix::time::parse

#endif // VIX_TIME_PARSE_HPP
