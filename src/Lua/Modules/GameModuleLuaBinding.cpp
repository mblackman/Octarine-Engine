#include "Lua/Modules/GameModuleLuaBinding.h"

#include <glm/glm.hpp>

#include "Components/CameraComponents.h"
#include "ECS/Registry.h"
#include "Game/Game.h"
#include "Game/GameConfig.h"
#include "Systems/ProjectileEmitSystem.h"

void LuaModuleBinding<GameModule>::install(sol::state& lua, Game& game) {
  lua.set_function("quit_game", &Game::Quit);

  lua.set_function("set_game_map_dimensions", [&game](const double width, const double height) {
    auto& gameConfig = game.GetRegistry()->Get<GameConfig>();
    gameConfig.playableAreaHeight = static_cast<float>(height);
    gameConfig.playableAreaWidth = static_cast<float>(width);
  });

  lua.set_function("get_game_map_dimensions", [&game]() {
    const auto& gameConfig = game.GetRegistry()->Get<GameConfig>();
    return glm::vec2(gameConfig.playableAreaWidth, gameConfig.playableAreaHeight);
  });

  lua.set_function("get_camera_position", [&game]() {
    const auto& camera = game.GetRegistry()->Get<CameraComponent>();
    return glm::vec2(camera.viewport.x, camera.viewport.y);
  });

  // Manual fire from Lua. Player-tagged emitters skip auto-fire — gameplay code drives shots
  // via input callbacks. dx/dy is a non-normalized aim vector; 0,0 falls back to the
  // emitter's configured velocity.
  lua.set_function(
      "fire_projectile", [&game](const Entity emitter, const sol::optional<double> dx, const sol::optional<double> dy) {
        auto* registry = game.GetRegistry();
        auto& projectileEmitSystem = registry->Get<ProjectileEmitSystem>();
        projectileEmitSystem.Fire(
            *registry, emitter, glm::vec2(static_cast<float>(dx.value_or(0.0)), static_cast<float>(dy.value_or(0.0))));
      });
}
