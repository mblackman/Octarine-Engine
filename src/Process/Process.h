#pragma once

// Process spawning wrapper used by editor-side tooling (Play, Export, deploy scripts, gh).
// RAII over the OS child handle; reader threads capture stdout/stderr into thread-safe
// buffers that the owner drains on the main thread via Pump() / Wait().

#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace octarine::process {
struct SpawnOptions {
  std::vector<std::string> argv;                         // argv[0] = program, resolved via PATH
  std::vector<std::pair<std::string, std::string>> env;  // empty = inherit fully
  std::string cwd;                                       // empty = inherit
  bool inherit_env = true;                               // false = env replaces fully; true = env extends
};

using OutputCallback = std::function<void(std::string_view)>;

class Process {
 public:
  Process();
  ~Process();

  Process(const Process&) = delete;
  Process& operator=(const Process&) = delete;
  Process(Process&&) noexcept;
  Process& operator=(Process&&) noexcept;

  // Launches the subprocess. Returns nullopt if the program could not be started (e.g. not
  // found on PATH); inspect the per-platform error via the OS later if needed.
  static std::optional<Process> Spawn(const SpawnOptions& opts);

  bool IsRunning();
  std::optional<int> ExitCode();
  void Kill();

  // Registers callbacks invoked on the calling thread of Pump() / Wait(). Multiple registers
  // overwrite the previous callback.
  void OnStdout(OutputCallback cb);
  void OnStderr(OutputCallback cb);

  // Drains any captured stdout/stderr and dispatches to the registered callbacks on the
  // current thread. Returns true if the process is still running.
  bool Pump();

  // Blocks until the child exits, then drains remaining output through Pump(). Returns the
  // exit code.
  int Wait();

 private:
  struct Impl;
  std::unique_ptr<Impl> impl_;
};
}  // namespace octarine::process
