#pragma once

#ifdef OCTARINE_WITH_EDITOR

// PlayerLauncher.
//
// Owns the editor's spawned player subprocess. The Run Player toolbar action goes through here,
// the per-frame Pump() call drains captured stdout/stderr into a ring-capped log buffer that the
// Player Output window reads, and Stop() sends a terminate to the child.
//
// Lifetime: stored as a Registry singleton on the engine's Registry. Game::Setup constructs it
// inside the OCTARINE_WITH_EDITOR-gated section; the destructor inherited from
// octarine::process::Process kills any still-running child at shutdown.

#include <cstddef>
#include <deque>
#include <filesystem>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "Process/Process.h"

namespace octarine::editor {
enum class PlayerStatus {
  Idle,
  Running,
  Exited,
  FailedToLaunch,
};

struct PlayerLogLine {
  bool is_stderr = false;
  std::string text;
};

class PlayerLauncher {
 public:
  // Resolves the player binary, spawns it pointed at `project_dir`, and starts capturing
  // its stdout/stderr. Returns false when the binary cannot be located or `posix_spawn` /
  // `CreateProcess` fails — both cases also push an explanatory line into the log buffer
  // and flip status to FailedToLaunch. A no-op (returning true) when a player is already
  // Running; v1 supports one concurrent player.
  bool Run(const std::filesystem::path& project_dir);

  // Terminates the running player if any. No-op when not Running.
  void Stop();

  // Drains the subprocess's captured stdout/stderr through the Process wrapper, splits the
  // chunks into lines, and updates `status_` / `last_exit_code_` when the child exits.
  // Cheap to call every frame; designed to live on the editor's main render path.
  void Pump();

  PlayerStatus Status() const { return status_; }
  std::optional<int> LastExitCode() const { return last_exit_code_; }
  const std::filesystem::path& ResolvedBinary() const { return resolved_binary_; }

  struct LogSnapshot {
    std::vector<PlayerLogLine> lines;
    std::size_t total_dropped = 0;  // lines dropped because the ring buffer was full
  };
  LogSnapshot LogCopy() const;
  void ClearLog();

  // Probes the dev-tree layout (editor next to its build preset, player in a sibling preset
  // dir) for `OctarineEngine-player[.exe]`. Public for unit testing — pass a synthetic
  // editor dir + filesystem layout to exercise the candidates without spawning anything.
  static std::optional<std::filesystem::path> ResolvePlayerBinary(const std::filesystem::path& editor_dir);

 private:
  static constexpr std::size_t kLogCap = 4096;

  std::optional<octarine::process::Process> process_;
  PlayerStatus status_ = PlayerStatus::Idle;
  std::optional<int> last_exit_code_;
  std::filesystem::path resolved_binary_;

  // Per-stream line splitter state: process chunks are not line-aligned, so we buffer the
  // tail until a '\n' arrives (or the process exits, at which point we flush).
  std::string stdout_partial_;
  std::string stderr_partial_;

  // No mutex — Process callbacks run synchronously inside Pump() on the calling thread,
  // so every append happens on the editor's main render thread.
  std::deque<PlayerLogLine> log_;
  std::size_t dropped_ = 0;

  void SplatChunk(std::string_view chunk, bool is_stderr);
  void AppendLine(bool is_stderr, std::string text);
  void FlushPartialLines();
};
}  // namespace octarine::editor

#endif  // OCTARINE_WITH_EDITOR
