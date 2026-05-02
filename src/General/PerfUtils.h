#pragma once
#include <chrono>
#include <source_location>
#include <string>
#include <utility>

#include "Logger.h"

namespace PerfUtils {
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
    const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - m_start).count();
    const auto ms = static_cast<double>(duration) * 0.001;

    const std::string finalName = m_name.empty() ? m_location.function_name() : m_name;
    Logger::Info("TIMER: " + finalName + ": " + std::to_string(ms) + "ms");
  }

 private:
  std::string m_name;
  std::source_location m_location;
  std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
};

#define PROFILE_SCOPE PerfUtils::ScopedTimer timer(__FUNCTION__)
#define PROFILE_NAMED_SCOPE(name) PerfUtils::ScopedTimer timer(name)

template <typename Func>
auto LogTime(const std::string &name, Func &&func) -> std::invoke_result_t<Func> {
  const auto start = std::chrono::high_resolution_clock::now();
  if constexpr (std::is_void_v<std::invoke_result_t<Func>>) {
    func();
    const auto end = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    const auto ms = static_cast<double>(duration) * 0.001;
    Logger::Info("TIMER: " + name + ": " + std::to_string(ms) + "ms");
    return;
  } else {
    auto result = func();
    const auto end = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    const auto ms = static_cast<double>(duration) * 0.001;
    Logger::Info("TIMER: " + name + ": " + std::to_string(ms) + "ms");
    return result;
  }
}
}  // namespace PerfUtils