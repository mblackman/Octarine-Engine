#pragma once

#ifdef OCTARINE_WITH_EDITOR

// ExportBuilder.
//
// Wraps the spawn of a project's scaffolded build script (`scripts/build-desktop.{sh,ps1}` for the
// host-OS desktop target, `scripts/build-android.{sh,ps1}` for Android). Mirrors PlayerLauncher: a
// Registry singleton with a state machine (Idle -> Building -> Succeeded/Failed), a ring-capped log
// of subprocess stdout/stderr, and a per-frame Pump() drained on the main thread.
//
// Deferred follow-ups: in-editor signing-credential storage (DPAPI/keychain), SHA256 of artifact,
// "Reveal in Explorer" / clipboard install hint. AndroidRelease signing creds flow via the editor
// process's existing env vars (OCTARINE_ANDROID_KEYSTORE_PATH, etc.).

#include <cstddef>
#include <deque>
#include <filesystem>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "Process/Process.h"

namespace octarine::editor {
enum class ExportStatus {
  Idle,
  Building,
  Succeeded,
  Failed,
};

enum class ExportTarget {
  Desktop,         // host-OS via scripts/build-desktop.{sh,ps1}
  AndroidDebug,    // assembleDebug via scripts/build-android.{sh,ps1} debug
  AndroidRelease,  // bundleRelease via scripts/build-android.{sh,ps1} release
};

struct ExportLogLine {
  bool is_stderr = false;
  std::string text;
};

struct ExportOptions {
  std::filesystem::path project_dir;
  ExportTarget target = ExportTarget::Desktop;
  std::string version_name;  // optional; empty -> falls through to project.ini
  std::string version_code;  // optional; empty -> falls through to project.ini
  std::string preset;        // Desktop only; empty -> emitted script default (ship-release)
};

class ExportBuilder {
 public:
  // Pre-flight checks before Run() can succeed. Returns the list of error messages; empty =
  // ready to build. Mirrors the policy enforced by the scaffolded script + Gradle/CMake
  // identity validators so the UI can surface failures inline.
  //   - project_dir contains project.ini, and ProjectIni::ValidateForShipping passes
  //   - the scaffolded build script for the target exists under <project>/scripts/
  static std::vector<std::string> Validate(const std::filesystem::path& project_dir,
                                           ExportTarget target = ExportTarget::Desktop);

  // Spawns the scaffolded build script for `opts.target` with env vars set per the
  // ExportOptions. Returns false when Validate() fails (errors are pushed into the log as
  // stderr lines and status flips to Failed) or when spawning the script fails. A no-op
  // (returning true) when a build is already Building.
  bool Run(const ExportOptions& opts);

  // Terminates the running script subprocess if any. No-op when not Building.
  void Stop();

  // Drains captured stdout/stderr into the log, updates status when the process exits.
  // Cheap to call every frame.
  void Pump();

  ExportStatus Status() const { return status_; }
  std::optional<int> LastExitCode() const { return last_exit_code_; }
  const std::filesystem::path& ResolvedScript() const { return resolved_script_; }

  struct LogSnapshot {
    std::vector<ExportLogLine> lines;
    std::size_t total_dropped = 0;
  };
  LogSnapshot LogCopy() const;
  void ClearLog();

  // Picks the script flavor (.ps1 on Windows, .sh elsewhere) for the requested target. Returns
  // nullopt if the script is missing. Public for unit testing.
  static std::optional<std::filesystem::path> ResolveBuildScript(const std::filesystem::path& project_dir,
                                                                 ExportTarget target = ExportTarget::Desktop);

 private:
  static constexpr std::size_t kLogCap = 4096;

  std::optional<octarine::process::Process> process_;
  ExportStatus status_ = ExportStatus::Idle;
  std::optional<int> last_exit_code_;
  std::filesystem::path resolved_script_;

  std::string stdout_partial_;
  std::string stderr_partial_;

  std::deque<ExportLogLine> log_;
  std::size_t dropped_ = 0;

  void SplatChunk(std::string_view chunk, bool is_stderr);
  void AppendLine(bool is_stderr, std::string text);
  void FlushPartialLines();
};
}  // namespace octarine::editor

#endif  // OCTARINE_WITH_EDITOR
