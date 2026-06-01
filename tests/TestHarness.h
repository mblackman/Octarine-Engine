// Minimal gtest-free check helper shared by the standalone engine tests. Mirrors the inline
// `g_failures` / `Check` pattern used by ProcessTest / ProjectIniTest, factored out so the newer
// ECS / EventBus / system tests don't each re-declare it. Each test's main() ends with
//   return octarine::test::Result();
// so the process exit code is 0 on success, 1 on any failed check.
#pragma once

#include <iostream>
#include <string>

namespace octarine::test {

inline int g_failures = 0;

inline void Check(const bool cond, const std::string& what) {
  if (cond) {
    std::cout << "  ok   " << what << "\n";
  } else {
    std::cerr << "  FAIL " << what << "\n";
    ++g_failures;
  }
}

inline int Result() { return g_failures == 0 ? 0 : 1; }

}  // namespace octarine::test
