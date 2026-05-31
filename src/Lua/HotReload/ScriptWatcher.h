#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

#ifndef OCTARINE_SHIPPED

// Mtime-polling file watcher. Single-threaded, called from the main loop. Track files we care
// about, ask Poll() periodically for the subset whose last_write_time advanced since the last
// call. Atomic-save editors (temp + rename) are handled because we resolve the canonical path
// at Track() time and re-stat it each Poll.
class ScriptWatcher {
 public:
  ScriptWatcher() = default;

  // Add a file to the watch set. Path is canonicalized; duplicates and unknown files are
  // silently ignored (the second case logs and stays untracked — caller bug, not user error).
  void Track(const std::string& absPath);

  // Drop a file from the watch set. No-op if untracked.
  void Untrack(const std::string& absPath);

  // Returns canonical paths whose mtime has advanced since the last Poll. Order is unspecified.
  // Files that disappeared between Track and Poll are skipped (no error) and re-armed on next
  // call.
  std::vector<std::string> Poll();

  [[nodiscard]] bool Empty() const { return mtimes_.empty(); }
  [[nodiscard]] std::size_t TrackedCount() const { return mtimes_.size(); }

 private:
  static std::string Canonical(const std::string& absPath);

  std::unordered_map<std::string, std::filesystem::file_time_type> mtimes_;
};

#else

// Shipped builds: zero-cost stub so callers compile without #ifdef sprinkling. Track/Untrack
// drop their args, Poll always returns empty.
class ScriptWatcher {
 public:
  void Track(const std::string&) {}
  void Untrack(const std::string&) {}
  std::vector<std::string> Poll() { return {}; }
  [[nodiscard]] bool Empty() const { return true; }
  [[nodiscard]] std::size_t TrackedCount() const { return 0; }
};

#endif
