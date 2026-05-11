#pragma once
#include <atomic>
#include <chrono>
#include <map>
#include <mutex>
#include <source_location>
#include <string>
#include <utility>

#include "Logger.h"

namespace PerfUtils {
inline std::string DurationToMs(std::chrono::time_point<std::chrono::high_resolution_clock> start,
                                std::chrono::time_point<std::chrono::high_resolution_clock> end) {
  const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
  const auto ms = static_cast<double>(duration) * 0.001;
  return std::to_string(ms);
}

class ScopedTimer {
 public:
  explicit ScopedTimer(std::string name = "", const std::source_location &loc = std::source_location::current())
      : m_name(std::move(name)), m_location(loc), m_start(std::chrono::high_resolution_clock::now()) {}

  ScopedTimer(const ScopedTimer &) = delete;
  ScopedTimer &operator=(const ScopedTimer &) = delete;

  ScopedTimer(ScopedTimer &&) = delete;
  ScopedTimer &operator=(ScopedTimer &&) = delete;

  ~ScopedTimer() {
    const auto end = std::chrono::high_resolution_clock::now();
    const std::string finalName = m_name.empty() ? m_location.function_name() : m_name;
    Logger::Info("TIMER: " + finalName + ": " + DurationToMs(m_start, end) + "ms");
  }

 private:
  std::string m_name;
  std::source_location m_location;
  std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
};

class ProfilingAccumulator {
 public:
  static void Add(const std::string &timerName, const long long duration_microseconds) {
    std::lock_guard lock(s_mutex);
    s_accumulated_times[timerName] += duration_microseconds;
  }

  static void Reset() {
    std::lock_guard lock(s_mutex);
    s_accumulated_times.clear();
    s_start = std::chrono::high_resolution_clock::now();
  }

  static void Report(const std::string &reportName) {
    std::lock_guard lock(s_mutex);
    const std::string header = "--- Accumulated Performance Report: " + reportName + " ---";
    Logger::Info(header);
    const auto end = std::chrono::high_resolution_clock::now();
    Logger::Info("ACCUM TOTAL: " + DurationToMs(s_start, end) + "ms");
    for (const auto &[name, total_duration] : s_accumulated_times) {
      const auto ms = static_cast<double>(total_duration.load()) * 0.001;
      Logger::Info("ACCUM: " + name + ": " + std::to_string(ms) + "ms");
    }
    Logger::Info(std::string(header.size(), '-'));
  }

  static std::map<std::string, long long> GetAccumulatedTimes() {
    std::lock_guard lock(s_mutex);
    std::map<std::string, long long> result;
    for (const auto &[name, total_duration] : s_accumulated_times) {
      result[name] = total_duration.load();
    }
    return result;
  }

  static void Clear() {
    std::lock_guard lock(s_mutex);
    s_accumulated_times.clear();
  }

 private:
  inline static std::mutex s_mutex;
  inline static std::chrono::time_point<std::chrono::high_resolution_clock> s_start;
  inline static std::map<std::string, std::atomic<long long>> s_accumulated_times;
};

class AccumulatingScopedTimer {
 public:
  explicit AccumulatingScopedTimer(std::string name)
      : m_name(std::move(name)), m_start(std::chrono::high_resolution_clock::now()) {}

  ~AccumulatingScopedTimer() {
    const auto end = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - m_start).count();
    ProfilingAccumulator::Add(m_name, duration);
  }

  AccumulatingScopedTimer(const AccumulatingScopedTimer &) = delete;
  AccumulatingScopedTimer &operator=(const AccumulatingScopedTimer &) = delete;
  AccumulatingScopedTimer(AccumulatingScopedTimer &&) = delete;
  AccumulatingScopedTimer &operator=(AccumulatingScopedTimer &&) = delete;

 private:
  std::string m_name;
  std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
};

class AggregateProfilingSession {
 public:
  explicit AggregateProfilingSession(std::string name) : m_name(std::move(name)) { ProfilingAccumulator::Reset(); }
  ~AggregateProfilingSession() { ProfilingAccumulator::Report(m_name); }

  AggregateProfilingSession(const AggregateProfilingSession &) = delete;
  AggregateProfilingSession &operator=(const AggregateProfilingSession &) = delete;
  AggregateProfilingSession(AggregateProfilingSession &&) = delete;
  AggregateProfilingSession &operator=(AggregateProfilingSession &&) = delete;

 private:
  std::string m_name;
};

// Frame-scoped integer counters. Separate from ProfilingAccumulator (which is durations) so
// the profiler UI / bench output can distinguish "how long did X take" from "how many X were
// there." Two flavors: Add (accumulates across the frame from multiple threads — fetch_add on
// an atomic, mutex only on first registration) and Set (single-writer snapshot, e.g. queue
// size at end of frame). Game::Update calls ResetValues() per frame to zero the slots without
// destroying them — keeping the atomic addresses stable lets RenderSpriteSystem cache pointers
// across frames.
class PerfCounters {
 public:
  static std::atomic<long long> *GetOrCreate(const std::string &name) {
    {
      std::lock_guard lock(s_mutex);
      if (const auto it = s_counters.find(name); it != s_counters.end()) {
        return it->second.get();
      }
      auto slot = std::make_unique<std::atomic<long long>>(0);
      auto *raw = slot.get();
      s_counters.emplace(name, std::move(slot));
      return raw;
    }
  }

  static void Add(const std::string &name, const long long delta = 1) {
    GetOrCreate(name)->fetch_add(delta, std::memory_order_relaxed);
  }

  static void Set(const std::string &name, const long long value) {
    GetOrCreate(name)->store(value, std::memory_order_relaxed);
  }

  // Zero every registered counter without destroying the slots. Called once per frame so
  // accumulated values are per-frame, while pointer handles cached by hot-path callers stay
  // valid across frames.
  static void ResetValues() {
    std::lock_guard lock(s_mutex);
    for (const auto &[name, counter] : s_counters) {
      counter->store(0, std::memory_order_relaxed);
    }
  }

  static void Clear() {
    std::lock_guard lock(s_mutex);
    s_counters.clear();
  }

  static std::map<std::string, long long> GetCounters() {
    std::lock_guard lock(s_mutex);
    std::map<std::string, long long> result;
    for (const auto &[name, counter] : s_counters) {
      result[name] = counter->load(std::memory_order_relaxed);
    }
    return result;
  }

  // Emit each counter as `COUNTER: name: value` on the logger. Intended to be called once per
  // frame so bench output gets the same per-frame granularity as `TIMER:` lines.
  static void Report() {
    std::lock_guard lock(s_mutex);
    for (const auto &[name, counter] : s_counters) {
      Logger::Info("COUNTER: " + name + ": " + std::to_string(counter->load(std::memory_order_relaxed)));
    }
  }

 private:
  inline static std::mutex s_mutex;
  inline static std::map<std::string, std::unique_ptr<std::atomic<long long>>> s_counters;
};

template <typename Func>
auto LogTime(const std::string &name, Func &&func) -> std::invoke_result_t<Func> {
  const auto start = std::chrono::high_resolution_clock::now();
  if constexpr (std::is_void_v<std::invoke_result_t<Func>>) {
    func();
    const auto end = std::chrono::high_resolution_clock::now();
    Logger::Info("TIMER: " + name + ": " + DurationToMs(start, end) + "ms");
    return;
  } else {
    auto result = func();
    const auto end = std::chrono::high_resolution_clock::now();
    Logger::Info("TIMER: " + name + ": " + DurationToMs(start, end) + "ms");
    return result;
  }
}
}  // namespace PerfUtils

// When OCTARINE_PROFILING is defined (via CMake), these macros create scoped timers
// that log system-level frame timing. When NOT defined, they compile to nothing.
#define OCT_CONCAT_INNER(a, b) a##b
#define OCT_CONCAT(a, b) OCT_CONCAT_INNER(a, b)
#ifdef OCTARINE_PROFILING
#define PROFILE_SCOPE PerfUtils::ScopedTimer OCT_CONCAT(timer_, __LINE__)(__FUNCTION__)
#define PROFILE_NAMED_SCOPE(name) PerfUtils::ScopedTimer OCT_CONCAT(timer_, __LINE__)(name)
#define ACCUMULATE_PROFILE_SCOPE(name) PerfUtils::AccumulatingScopedTimer OCT_CONCAT(accTimer_, __LINE__)(name)
#define AGGREGATE_PROFILE_SESSION(name) PerfUtils::AggregateProfilingSession OCT_CONCAT(aggSession_, __LINE__)(name)
#define PROFILE_COUNTER_ADD(name, delta) PerfUtils::PerfCounters::Add((name), (delta))
#define PROFILE_COUNTER_SET(name, value) PerfUtils::PerfCounters::Set((name), (value))
#define PROFILE_COUNTER_HANDLE(name) PerfUtils::PerfCounters::GetOrCreate(name)
#define PROFILE_COUNTER_INC(handle)                                     \
  do {                                                                  \
    if (auto *h = (handle)) h->fetch_add(1, std::memory_order_relaxed); \
  } while (0)
#define PROFILE_COUNTERS_REPORT() PerfUtils::PerfCounters::Report()
#else
#define PROFILE_SCOPE ((void)0)
#define PROFILE_NAMED_SCOPE(name) ((void)0)
#define ACCUMULATE_PROFILE_SCOPE(name) ((void)0)
#define AGGREGATE_PROFILE_SESSION(name) ((void)0)
#define PROFILE_COUNTER_ADD(name, delta) ((void)0)
#define PROFILE_COUNTER_SET(name, value) ((void)0)
#define PROFILE_COUNTER_HANDLE(name) (nullptr)
#define PROFILE_COUNTER_INC(handle) ((void)0)
#define PROFILE_COUNTERS_REPORT() ((void)0)
#endif