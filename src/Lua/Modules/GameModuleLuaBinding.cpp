#include "Lua/Modules/GameModuleLuaBinding.h"

#include <glm/glm.hpp>
#include <sstream>
#include <string>

#include "Components/CameraComponents.h"
#include "ECS/Registry.h"
#include "Game/GameConfig.h"
#include "General/Logger.h"
#include "Lua/LuaBindingContext.h"
#include "Systems/ProjectileEmitSystem.h"

namespace {
std::string FormatLuaTable(const sol::table& table, int indent_level = 0, int max_depth = 5) {
  if (indent_level > max_depth) {
    return "{ ... }";
  }
  std::ostringstream result;
  std::string indent(indent_level * 2, ' ');
  std::string inner_indent((indent_level + 1) * 2, ' ');
  result << "{\n";
  for (const auto& kv : table) {
    result << inner_indent;

    // Key
    if (kv.first.get_type() == sol::type::string) {
      result << kv.first.as<std::string>();
    } else if (kv.first.get_type() == sol::type::number) {
      result << "[" << kv.first.as<double>() << "]";
    } else {
      result << "[" << lua_typename(table.lua_state(), static_cast<int>(kv.first.get_type())) << "]";
    }

    result << " = ";

    // Value
    if (kv.second.get_type() == sol::type::string) {
      result << "\"" << kv.second.as<std::string>() << "\"";
    } else if (kv.second.get_type() == sol::type::number) {
      result << kv.second.as<double>();
    } else if (kv.second.get_type() == sol::type::boolean) {
      result << (kv.second.as<bool>() ? "true" : "false");
    } else if (kv.second.get_type() == sol::type::table) {
      result << FormatLuaTable(kv.second.as<sol::table>(), indent_level + 1, max_depth);
    } else {
      result << "<" << lua_typename(table.lua_state(), static_cast<int>(kv.second.get_type())) << ">";
    }
    result << ",\n";
  }
  result << indent << "}";
  return result.str();
}
}  // namespace

void LuaModuleBinding<GameModule>::install(sol::state& lua, LuaBindingContext& ctx) {
  lua.set_function("log_table", [](const sol::table& table, const sol::optional<std::string>& message) {
    std::string prefix = message.has_value() ? (message.value() + ": ") : "";
    Logger::InfoLua(prefix + FormatLuaTable(table));
  });

  lua.set_function("quit_game", [&ctx]() { ctx.RequestQuit(); });

  // Toggle the built-in performance overlay (FPS + frame time) at runtime. Placement and which
  // metrics show are config.ini knobs (PerfOverlayCorner / PerfOverlayMetrics); this just flips it on
  // or off so scripts can gate it behind a debug menu or key.
  lua.set_function("set_perf_overlay", [&ctx](const bool enabled) {
    ctx.GetRegistry()->Get<GameConfig>().GetEngineOptions().showPerfOverlay = enabled;
  });

  // Flip the perf overlay and return the new state, so a debug-key callback doesn't have to track
  // the current value itself.
  lua.set_function("toggle_perf_overlay", [&ctx]() {
    auto& options = ctx.GetRegistry()->Get<GameConfig>().GetEngineOptions();
    options.showPerfOverlay = !options.showPerfOverlay;
    return options.showPerfOverlay;
  });

  lua.set_function("set_game_map_dimensions", [&ctx](const double width, const double height) {
    auto& gameConfig = ctx.GetRegistry()->Get<GameConfig>();
    gameConfig.playableAreaHeight = static_cast<float>(height);
    gameConfig.playableAreaWidth = static_cast<float>(width);
  });

  lua.set_function("get_game_map_dimensions", [&ctx]() {
    const auto& gameConfig = ctx.GetRegistry()->Get<GameConfig>();
    return glm::vec2(gameConfig.playableAreaWidth, gameConfig.playableAreaHeight);
  });

  lua.set_function("get_camera_position", [&ctx]() {
    const auto& camera = ctx.GetRegistry()->Get<CameraComponent>();
    return glm::vec2(camera.viewport.x, camera.viewport.y);
  });

  // Manual fire from Lua. Player-tagged emitters skip auto-fire — gameplay code drives shots
  // via input callbacks. dx/dy is a non-normalized aim vector; 0,0 falls back to the
  // emitter's configured velocity.
  lua.set_function(
      "fire_projectile", [&ctx](const Entity emitter, const sol::optional<double> dx, const sol::optional<double> dy) {
        auto* registry = ctx.GetRegistry();
        auto& projectileEmitSystem = registry->Get<ProjectileEmitSystem>();
        projectileEmitSystem.Fire(
            *registry, emitter, glm::vec2(static_cast<float>(dx.value_or(0.0)), static_cast<float>(dy.value_or(0.0))));
      });
}
