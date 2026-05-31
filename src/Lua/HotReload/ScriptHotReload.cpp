#include "ScriptHotReload.h"

#ifndef OCTARINE_SHIPPED

#include <filesystem>
#include <system_error>

#include "../../Components/ScriptComponent.h"
#include "../../ECS/Query.h"
#include "../../ECS/Registry.h"
#include "../../General/Logger.h"
#include "../Bindings/LuaBinding.h"

namespace {
std::string Canonicalize(const std::string& absPath) {
  std::error_code ec;
  auto canon = std::filesystem::weakly_canonical(std::filesystem::path(absPath), ec);
  if (ec) {
    return absPath;
  }
  return canon.string();
}
}  // namespace

void ScriptHotReload::Tick(Registry& registry, sol::state& lua, float dt, float pollSeconds) {
  accumulator_ += dt;
  if (accumulator_ < pollSeconds) {
    return;
  }
  accumulator_ = 0.0F;

  RefreshTracking(registry);
  if (watcher_.Empty()) {
    return;
  }
  for (const auto& dirty : watcher_.Poll()) {
    ReloadFile(dirty, registry, lua);
  }
}

void ScriptHotReload::ForceReloadAll(Registry& registry, sol::state& lua) {
  std::unordered_set<std::string> paths;
  auto query = registry.CreateQuery<ScriptComponent>();
  query->ForEach([&](const ScriptComponent& sc) {
    if (!sc.sourcePath.empty()) {
      paths.insert(Canonicalize(sc.sourcePath));
    }
  });
  for (const auto& p : paths) {
    ReloadFile(p, registry, lua);
  }
}

void ScriptHotReload::RefreshTracking(Registry& registry) {
  std::unordered_set<std::string> seen;
  auto query = registry.CreateQuery<ScriptComponent>();
  query->ForEach([&](const ScriptComponent& sc) {
    if (!sc.sourcePath.empty()) {
      seen.insert(Canonicalize(sc.sourcePath));
    }
  });

  for (const auto& path : seen) {
    if (tracked_.find(path) == tracked_.end()) {
      watcher_.Track(path);
      tracked_.insert(path);
    }
  }
  for (auto it = tracked_.begin(); it != tracked_.end();) {
    if (seen.find(*it) == seen.end()) {
      watcher_.Untrack(*it);
      it = tracked_.erase(it);
    } else {
      ++it;
    }
  }
}

void ScriptHotReload::ReloadFile(const std::string& canonicalPath, Registry& registry, sol::state& lua) {
  // protected_function (not plain sol::function) so Lua errors return invalid results instead of
  // propagating as C++ exceptions — engine + tests compile without /EHsc. Explicit .get<>()
  // dodges GCC -Wconversion ambiguity between operator T() and the ctor overload.
  auto dofile = lua["dofile"].get<sol::protected_function>();
  if (!dofile.valid()) {
    RecordFailure(canonicalPath, "dofile not installed");
    return;
  }

  sol::protected_function_result result = dofile(canonicalPath);
  if (!result.valid()) {
    const auto err = result.get<sol::error>();
    RecordFailure(canonicalPath, err.what());
    return;
  }

  const auto returned = result.get<sol::object>();
  if (!returned.is<sol::table>()) {
    RecordFailure(canonicalPath, "script did not return a table");
    return;
  }
  const sol::table newTable = returned.as<sol::table>();

  int swapped = 0;
  auto query = registry.CreateQuery<ScriptComponent>();
  query->ForEach([&](ScriptComponent& sc) {
    if (Canonicalize(sc.sourcePath) == canonicalPath) {
      if (SwapComponent(sc, newTable)) {
        ++swapped;
      }
    }
  });
  RecordSuccess(canonicalPath);
  Logger::Info("Hot reload: " + canonicalPath + " (" + std::to_string(swapped) + " entities)");
}

bool ScriptHotReload::SwapComponent(ScriptComponent& sc, const sol::table& newTable) {
  using namespace LuaComponentHelpers;
  // Preserve per-entity state (`data` sub-table) across the swap. Authors mutate this at
  // runtime; clobbering it would defeat the point of hot reload.
  const auto preservedData = sc.scriptTable.get<sol::object>("data");

  // Resolve new callbacks first so we never end up with a half-swapped state if Lua throws.
  sol::protected_function newUpdate = SafeGetProtectedFunction(newTable, "on_update");
  sol::protected_function newDebugGui = SafeGetProtectedFunction(newTable, "on_debug_gui");

  // Drop old protected_function refs before assigning the new table — pins on the old chunk
  // would otherwise leak across reloads (sol2 holds Lua-side registry refs).
  sc.updateFunction = sol::lua_nil;
  sc.onDebugGUIFunction = sol::lua_nil;
  sc.scriptTable = newTable;
  if (preservedData.valid() && preservedData.get_type() != sol::type::lua_nil) {
    sc.scriptTable["data"] = preservedData;
  }
  sc.updateFunction = newUpdate;
  sc.onDebugGUIFunction = newDebugGui;
  return true;
}

void ScriptHotReload::RecordSuccess(const std::string& path) {
  last_.path = path;
  last_.error.clear();
  ++last_.totalReloads;
}

void ScriptHotReload::RecordFailure(const std::string& path, const std::string& error) {
  last_.path = path;
  last_.error = error;
  ++last_.totalFailures;
  Logger::ErrorLua("Hot reload failed for '" + path + "': " + error);
}

#endif  // OCTARINE_SHIPPED
