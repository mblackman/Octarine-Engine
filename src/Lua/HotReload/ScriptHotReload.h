#pragma once

#include <sol/sol.hpp>
#include <string>
#include <unordered_set>

#include "General/FileWatcher.h"

class Registry;
struct ScriptComponent;

#ifndef OCTARINE_SHIPPED

// Owns the watcher + auto-discovers file-backed ScriptComponents via a Registry query each tick.
// One instance lives in the Registry singleton set; ticked from the main loop. Inline
// ScriptComponents (empty sourcePath) are ignored.
class ScriptHotReload {
 public:
  struct ReloadInfo {
    std::string path;   // empty until first reload
    std::string error;  // non-empty if last reload failed
    int totalReloads = 0;
    int totalFailures = 0;
  };

  ScriptHotReload() = default;

  // Called from main loop. dt is real time (not time-scaled) so paused editor sessions still
  // iterate. lua state must be the same one ScriptComponents bound their tables against.
  void Tick(Registry& registry, sol::state& lua, float dt, float pollSeconds);

  // Forced reload of every file-backed ScriptComponent, ignoring mtime.
  void ForceReloadAll(Registry& registry, sol::state& lua);

  [[nodiscard]] const ReloadInfo& LastReload() const { return last_; }

 private:
  void RefreshTracking(Registry& registry);
  void ReloadFile(const std::string& canonicalPath, Registry& registry, sol::state& lua);
  bool SwapComponent(ScriptComponent& sc, const sol::table& newTable);
  void RecordSuccess(const std::string& path);
  void RecordFailure(const std::string& path, const std::string& error);

  FileWatcher watcher_;
  std::unordered_set<std::string> tracked_;  // canonical paths the watcher currently watches
  float accumulator_ = 0.0F;
  ReloadInfo last_;
};

#else

// Shipped stub.
class ScriptHotReload {
 public:
  struct ReloadInfo {};
  void Tick(Registry&, sol::state&, float, float) {}
  void ForceReloadAll(Registry&, sol::state&) {}
  [[nodiscard]] ReloadInfo LastReload() const { return {}; }
};

#endif
