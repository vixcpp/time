/**
 * @file basic_time.cpp
 *
 * Basic sanity tests for the Vix time module.
 */

#include <cassert>
#include <iostream>

#include <vix/time/time.hpp>

using namespace vix::time;

int main()
{
  // --------------------------------------------------
  // Duration
  // --------------------------------------------------

  Duration d1 = Duration::seconds(2);
  Duration d2 = Duration::milliseconds(500);

  assert(d1.count_seconds() == 2);
  assert(d2.count_ms() == 500);
  assert((d1 + d2).count_ms() == 2500);

  // --------------------------------------------------
  // Timestamp
  // --------------------------------------------------

  Timestamp t0 = Timestamp::now();
  Timestamp t1 = t0 + Duration::seconds(1);

  Duration delta = t1 - t0;
  assert(delta.count_seconds() == 1);

  // Ordering
  assert(t1 > t0);

  // --------------------------------------------------
  // Date
  // --------------------------------------------------

  Date d = Date::parse("2026-02-07");
  assert(d.year() == 2026);
  assert(d.month() == 2);
  assert(d.day() == 7);
  assert(d.to_string() == "2026-02-07");

  Timestamp day_start = d.to_timestamp_utc();
  assert(day_start.seconds_since_epoch() > 0);

  // --------------------------------------------------
  // DateTime
  // --------------------------------------------------

  DateTime dt = DateTime::parse("2026-02-07T10:30:15Z");
  assert(dt.year() == 2026);
  assert(dt.month() == 2);
  assert(dt.day() == 7);
  assert(dt.hour() == 10);
  assert(dt.minute() == 30);
  assert(dt.second() == 15);

  Timestamp ts = dt.to_timestamp_utc();
  DateTime roundtrip = DateTime::from_timestamp_utc(ts);
  assert(roundtrip.to_string_utc() == dt.to_string_utc());

  // --------------------------------------------------
  // Steady clock
  // --------------------------------------------------

  auto start = SteadyClock::now_chrono();
  int sink = 0;
  for (int i = 0; i < 100'000; ++i)
    sink += i;
  Duration elapsed = SteadyClock::since(start);

  assert(elapsed.count_ns() >= 0);

  std::cout << "[time] basic_time tests passed\n";
  return 0;
}
