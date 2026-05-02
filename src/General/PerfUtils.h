#pragma once
#include <chrono>
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
  static void Add(const std::string &name, const long long duration_microseconds) {
    std::lock_guard lock(s_mutex);
    s_accumulated_times[name] += duration_microseconds;
  }

  static void Reset() {
    std::lock_guard lock(s_mutex);
    s_accumulated_times.clear();
    s_start = std::chrono::high_resolution_clock::now();
  }

  static void Report(const std::string &name) {
    std::lock_guard lock(s_mutex);
    const std::string header = "--- Accumulated Performance Report: " + name + " ---";
    Logger::Info(header);
    const auto end = std::chrono::high_resolution_clock::now();
    Logger::Info("ACCUM TOTAL: " + DurationToMs(s_start, end) + "ms");
    for (const auto &[name, total_duration] : s_accumulated_times) {
      const auto ms = static_cast<double>(total_duration.load()) * 0.001;
      Logger::Info("ACCUM: " + name + ": " + std::to_string(ms) + "ms");
    }
    Logger::Info(std::string(header.size(), '-'));
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

#define PROFILE_SCOPE PerfUtils::ScopedTimer timer(__FUNCTION__)
#define PROFILE_NAMED_SCOPE(name) PerfUtils::ScopedTimer timer(name)

#define ACCUMULATE_PROFILE_SCOPE(name) PerfUtils::AccumulatingScopedTimer accTimer##__LINE__(name)
#define AGGREGATE_PROFILE_SESSION(name) PerfUtils::AggregateProfilingSession aggSession##__LINE__(name)