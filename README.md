# vix::time

**Time and date utilities for Vix.cpp.**

`vix::time` provides a simple, explicit, and predictable API for working with
dates, times, timestamps, and durations in modern C++.

The design is inspired by the ergonomics of Node.js and Python, while remaining
fully type-safe, chrono-based, and explicit.

---

## Features

- 📅 **Date facade** (`Date::now()`, `Date::parse()`)
- 🕒 **DateTime** with UTC support and ISO-8601 parsing
- ⏱ **Timestamp** (epoch-based, nanosecond precision)
- ⌛ **Duration** (strongly typed, chrono-backed)
- 🧭 **System & Steady clocks**
- 📦 Header-only (v1)
- 🚫 No hidden allocations, no magic, no dependencies

## Benchmarks

Benchmarks are built using an internal chrono-based harness
(no external dependencies).

Build:
```bash
cmake -S . -B build -DVIX_TIME_BUILD_BENCH=ON
cmake --build build
```
Run:

```bash
./build/src/bench/vix_time_bench
```

#### Example results (Linux, x86_64):

- Date.parse("YYYY-MM-DD"): ~170 ns (p50)
- DateTime.parse ISO-8601: ~1.1 µs (p50)
- Timestamp.now(): ~65 ns (p50)
- Date.to_timestamp_utc(): ~240 ns (p50)

### These numbers make vix::time suitable for:

- logging
- tracing
- WAL
- retry / timeout logic
- scheduling

---

## Installation

`vix::time` is designed to be used as part of the Vix umbrella project.

```cmake
add_subdirectory(modules/time)
```

Or link against the exported target:

```cmake
target_link_libraries(your_target PRIVATE vix::time)
```

---

## Quick Example

```cpp
#include <vix/time/time.hpp>
#include <iostream>

using namespace vix::time;

int main()
{
  Date today = Date::now();
  std::cout << today.to_string() << "\n";

  DateTime now = DateTime::now_utc();
  std::cout << now.to_string_utc() << "\n";

  Timestamp t0 = Timestamp::now();
  Timestamp t1 = t0 + Duration::seconds(1);

  Duration delta = t1 - t0;
  std::cout << delta.count_seconds() << "s\n";
}
```

---

## API Overview

### Date

```cpp
Date::now();
Date::today();
Date::parse("YYYY-MM-DD");
```

Represents a calendar date without time.

### DateTime

```cpp
DateTime::now_utc();
DateTime::parse("YYYY-MM-DDTHH:MM:SSZ");
```

Represents a full date + time in UTC.

### Timestamp

```cpp
Timestamp::now();
Timestamp::from_seconds(1700000000);
```

Absolute point in time (nanoseconds since Unix epoch).

### Duration

```cpp
Duration::seconds(5);
Duration::milliseconds(250);
```

Strongly typed time spans.

### Clocks

```cpp
SystemClock::now();          // wall time (Timestamp)
SteadyClock::now_chrono();   // monotonic clock
SteadyClock::since(start);   // elapsed Duration
```

---

## Design Principles

- Explicit over implicit
- Strong types instead of raw integers
- Predictable behavior
- Minimal surface API
- No hidden global state

`vix::time` is intended to be a foundational module used by:

- sync
- cache
- WAL
- retry / timeout logic
- scheduling

---

## Status

- **Version**: v0.1.0
- **Mode**: Header-only
- **C++ Standard**: C++20
- **Stability**: Experimental (API subject to refinement)

---

## License

MIT License
Copyright © 2026 Gaspard Kirira
