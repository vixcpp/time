#ifndef VIX_TIME_BENCH_HPP
#define VIX_TIME_BENCH_HPP

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <string>
#include <utility>
#include <vector>
#include <iostream>

namespace vix::time::bench
{
  namespace detail
  {
#if defined(__GNUC__) || defined(__clang__)
    inline void blackhole() { asm volatile("" ::: "memory"); }
    template <typename T>
    inline void blackhole(T const &v) { asm volatile("" : : "g"(v) : "memory"); }
#else
    inline void blackhole() {}
    template <typename T>
    inline void blackhole(T const &) {}
#endif
  }

  struct Stats
  {
    std::int64_t iters = 0;
    std::int64_t min_ns = 0;
    std::int64_t p50_ns = 0;
    std::int64_t p95_ns = 0;
    std::int64_t max_ns = 0;
    double avg_ns = 0.0;
  };

  inline Stats compute(std::vector<std::int64_t> samples_ns)
  {
    Stats s{};
    if (samples_ns.empty())
      return s;

    std::sort(samples_ns.begin(), samples_ns.end());
    s.iters = (std::int64_t)samples_ns.size();
    s.min_ns = samples_ns.front();
    s.max_ns = samples_ns.back();

    auto idx = [&](double p) -> std::size_t
    {
      double pos = p * double(samples_ns.size() - 1);
      return (std::size_t)(pos + 0.5);
    };

    s.p50_ns = samples_ns[idx(0.50)];
    s.p95_ns = samples_ns[idx(0.95)];

    long double sum = 0;
    for (auto v : samples_ns)
      sum += (long double)v;
    s.avg_ns = (double)(sum / (long double)samples_ns.size());
    return s;
  }

  template <typename Fn>
  inline Stats run(std::string name, Fn fn,
                   int warmup = 1000,
                   int iters = 20000)
  {
    using clock = std::chrono::steady_clock;
    using ns = std::chrono::nanoseconds;

    for (int i = 0; i < warmup; ++i)
    {
      fn();
      detail::blackhole();
    }

    std::vector<std::int64_t> samples;
    samples.reserve((std::size_t)iters);

    for (int i = 0; i < iters; ++i)
    {
      auto t0 = clock::now();
      fn();
      auto t1 = clock::now();

      detail::blackhole();
      auto dt = std::chrono::duration_cast<ns>(t1 - t0).count();
      samples.push_back((std::int64_t)dt);
    }

    auto st = compute(std::move(samples));

    std::cout << "[bench] " << name
              << " iters=" << st.iters
              << " min=" << st.min_ns << "ns"
              << " p50=" << st.p50_ns << "ns"
              << " p95=" << st.p95_ns << "ns"
              << " avg=" << (std::int64_t)st.avg_ns << "ns"
              << " max=" << st.max_ns << "ns"
              << "\n";

    return st;
  }
}

#endif
