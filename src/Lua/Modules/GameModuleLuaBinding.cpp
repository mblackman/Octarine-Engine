#include "Lua/Modules/GameModuleLuaBinding.h"

#include <glm/glm.hpp>

#include "Components/CameraComponents.h"
#include "ECS/Registry.h"
#include "Game/GameConfig.h"
#include "Lua/LuaBindingContext.h"
#include "Systems/ProjectileEmitSystem.h"

void LuaModuleBinding<GameModule>::install(sol::state& lua, LuaBindingContext& ctx) {
  lua.set_function("quit_game", [&ctx]() { ctx.RequestQuit(); });

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
