#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

#ifndef OCTARINE_SHIPPED

// Polls file modification timestamps on the main loop for hot reloading.
class FileWatcher {
 public:
  FileWatcher() = default;

  // Adds canonical file path to watch set.
  void Track(const std::string& absPath);

  // Removes file from watch set.
  void Untrack(const std::string& absPath);

  // Returns paths whose last write time advanced since the last poll.
  std::vector<std::string> Poll();

  [[nodiscard]] bool Empty() const { return mtimes_.empty(); }
  [[nodiscard]] std::size_t TrackedCount() const { return mtimes_.size(); }

 private:
  static std::string Canonical(const std::string& absPath);

  std::unordered_map<std::string, std::filesystem::file_time_type> mtimes_;
};

#else

// Stub implementation for shipped builds.
class FileWatcher {
 public:
  void Track(const std::string&) {}
  void Untrack(const std::string&) {}
  std::vector<std::string> Poll() { return {}; }
  [[nodiscard]] bool Empty() const { return true; }
  [[nodiscard]] std::size_t TrackedCount() const { return 0; }
};

#endif
