// Smoke checks for the process-spawn wrapper. gtest-free; failed-check count is exit code.
// Mirrors the style of LuaApiSmokeTest / AssetPipelineTest. Registered as ProcessTest in ctest.
//
// Hard skip: if `cmake` is not on PATH the executable returns 0 with a SKIP line on stderr —
// CI environments without cmake on PATH should not flunk this test.

#include <chrono>
#include <iostream>
#include <string>
#include <thread>

#include "Process/Process.h"

namespace {
int g_failures = 0;

void Check(const bool cond, const std::string& what) {
  if (cond) {
    std::cout << "  ok   " << what << "\n";
  } else {
    std::cerr << "  FAIL " << what << "\n";
    ++g_failures;
  }
}
}  // namespace

int main() {
  using namespace octarine::process;

  {
    SpawnOptions opts;
    opts.argv = {"cmake", "--version"};
    auto proc = Process::Spawn(opts);
    if (!proc) {
      std::cerr << "SKIP: `cmake` not available on PATH\n";
      return 0;
    }

    std::string stdout_capture;
    proc->OnStdout([&](std::string_view sv) { stdout_capture.append(sv); });

    const int code = proc->Wait();
    Check(code == 0, "cmake --version exits with code 0");
    Check(stdout_capture.find("cmake version") != std::string::npos, "stdout contains 'cmake version'");
  }

  {
    SpawnOptions opts;
    opts.argv = {"definitely-not-a-real-binary-octarine"};
    auto proc = Process::Spawn(opts);
    Check(!proc.has_value(), "spawning a missing program returns nullopt");
  }

  {
    // Argv quoting: a single arg containing spaces and quotes round-trips. cmake's -E echo
    // prints argv joined by spaces; the engine layer only cares that argv survives intact.
    // We round-trip through `cmake -E echo` so this same test exercises the Windows quoting
    // path end-to-end.
    SpawnOptions opts;
    opts.argv = {"cmake", "-E", "echo", "hello \"weird\" world"};
    auto proc = Process::Spawn(opts);
    if (proc) {
      std::string out;
      proc->OnStdout([&](std::string_view sv) { out.append(sv); });
      const int code = proc->Wait();
      Check(code == 0, "cmake -E echo exits cleanly with quoted arg");
      Check(out.find("hello \"weird\" world") != std::string::npos,
            "argv quoting preserves embedded quotes and spaces");
    }
  }

  return g_failures == 0 ? 0 : 1;
}
