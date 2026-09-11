// Smoke checks for the process-spawn wrapper. gtest-free; failed-check count is exit code.
// Mirrors the style of LuaApiSmokeTest / AssetPipelineTest. Registered as ProcessTest in ctest.
//
// Hard skip: if `cmake` is not on PATH the executable returns 0 with a SKIP line on stderr —
// CI environments without cmake on PATH should not flunk this test.

#include <chrono>
#include <filesystem>
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
  }
  if (!cond) {
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

  {
    // CWD test: verify running in a specified directory creates files there.
    const auto temp_dir = std::filesystem::temp_directory_path();
    const auto marker_name = "octarine_proc_cwd_marker.tmp";
    const auto marker_path = temp_dir / marker_name;
    std::error_code ec;
    std::filesystem::remove(marker_path, ec);

    SpawnOptions opts;
    opts.argv = {"cmake", "-E", "touch", marker_name};
    opts.cwd = temp_dir.string();
    auto proc = Process::Spawn(opts);
    if (proc) {
      const int code = proc->Wait();
      Check(code == 0, "cmake -E touch with opts.cwd exits cleanly");
      Check(std::filesystem::exists(marker_path), "opts.cwd respected: file created in target dir");
      std::filesystem::remove(marker_path, ec);
    }
  }

  return g_failures == 0 ? 0 : 1;
}
