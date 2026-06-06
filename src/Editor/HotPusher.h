#pragma once

#ifdef OCTARINE_WITH_EDITOR

#include <cstddef>
#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

namespace octarine::editor {

struct PushTarget {
  enum class Kind { DevListen, Adb };
  Kind kind = Kind::DevListen;
  std::string address;  // host:port for DevListen, serial for Adb
  bool auto_push = false;
  // Status (written by Tick/PushAll, read by UI)
  bool last_ok = true;
  std::size_t last_bytes = 0;
  double last_push_time = -1.0;  // < 0 = never
  std::string last_message;
};

class HotPusher {
 public:
  // Tick: call each frame. Polls every 250ms; pushes changed files to auto-push targets.
  // project_dir: current project assets dir (empty = no project).
  void Tick(double imgui_time, const std::filesystem::path& project_dir);

  // Immediately push all project files to one target (manual push button).
  void PushAll(const std::string& address, const std::filesystem::path& project_dir);

  // Target management (called by Devices panel on device connect/disconnect).
  void UpsertTarget(const std::string& address, PushTarget::Kind kind);
  void RemoveTarget(const std::string& address);
  PushTarget* FindTarget(const std::string& address);

  std::vector<PushTarget>& Targets() { return targets_; }

 private:
  std::filesystem::path project_dir_;
  std::string package_id_;
  std::unordered_map<std::string, std::filesystem::file_time_type> manifest_;
  std::vector<PushTarget> targets_;
  double last_poll_time_ = -999.0;
  static constexpr double kPollInterval = 0.25;

  void UpdateProjectDir(const std::filesystem::path& dir);
  std::vector<std::filesystem::path> CollectChangedFiles();
  static std::vector<char> ReadFile(const std::filesystem::path& path);
  std::size_t PushFilesToTarget(PushTarget& target, const std::vector<std::filesystem::path>& files, bool reload_after);
  std::size_t PushAllFilesToTarget(PushTarget& target);
};

}  // namespace octarine::editor
#endif  // OCTARINE_WITH_EDITOR
