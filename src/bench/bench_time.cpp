#include <vix/time/time.hpp>
#include <vix/time/bench/Bench.hpp>
#include <string>

using namespace vix::time;

int main()
{
  using vix::time::bench::run;

  run("Date.parse YYYY-MM-DD", []()
      {
        auto d = Date::parse("2026-02-07");
        vix::time::bench::detail::blackhole(d.year()); });

  run("DateTime.parse ISO Z", []()
      {
        auto dt = DateTime::parse("2026-02-07T10:30:15.123456789Z");
        vix::time::bench::detail::blackhole(dt.second()); });

  run("Timestamp.now", []()
      {
        auto t = Timestamp::now();
        vix::time::bench::detail::blackhole(t.seconds_since_epoch()); });

  run("Date.to_timestamp_utc", []()
      {
        Date d = Date::parse("2026-02-07");
        auto ts = d.to_timestamp_utc();
        vix::time::bench::detail::blackhole(ts.seconds_since_epoch()); });

  return 0;
}
