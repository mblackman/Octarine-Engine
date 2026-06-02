// Minimal gtest-free check helper shared by the standalone engine tests. Mirrors the inline
// `g_failures` / `Check` pattern used by ProcessTest / ProjectIniTest, factored out so the newer
// ECS / EventBus / system tests don't each re-declare it. Each test's main() ends with
//   return octarine::test::Result();
// so the process exit code is 0 on success, 1 on any failed check.
#pragma once

#include <iostream>
#include <sstream>
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

// Equality check that prints the offending values on failure. Both sides must be ostream-streamable.
template <typename A, typename B>
void CheckEq(const A& got, const B& want, const std::string& what) {
  if (got == want) {
    std::cout << "  ok   " << what << "\n";
  } else {
    std::ostringstream os;
    os << "  FAIL " << what << " (got=" << got << ", want=" << want << ")";
    std::cerr << os.str() << "\n";
    ++g_failures;
  }
}

inline int Result() { return g_failures == 0 ? 0 : 1; }

// Named variant: prints a PASS/FAIL summary line for `suiteName` and returns the failure count
// (0 = success) so a test's main() can `return octarine::test::ReportSummary("...");`.
inline int ReportSummary(const std::string& suiteName) {
  if (g_failures == 0) {
    std::cout << "\n" << suiteName << ": PASS\n";
  } else {
    std::cerr << "\n" << suiteName << ": " << g_failures << " FAILED\n";
  }
  return g_failures;
}

}  // namespace octarine::test
