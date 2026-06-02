#pragma once

#include <SDL3/SDL.h>

#include "../General/Logger.h"
#include "Components/ScriptComponent.h"
#include "ECS/Iterable.h"
#include "ECS/Registry.h"

#ifdef OCTARINE_WITH_IMGUI

class Game;

// Per-frame ImGui pass. In editor builds it delegates the whole editor chrome to
// Editor/Panels/EditorPanelHost; here it owns only the game-facing overlays (the script
// onDebugGUI hook, FPS plot, entity count) and the ImGui frame begin/end that both the editor
// and the IMGUI-only player-debug build share.
class RenderDebugGUISystem {
 public:
  void operator()(const ContextFacade& context, ScriptComponent& script) const {
    if (script.onDebugGUIFunction == sol::lua_nil) {
      return;
    }
    if (auto result = script.onDebugGUIFunction(script.scriptTable, context.GetEntity()); !result.valid()) {
      const sol::error err = result;
      Logger::ErrorLua(std::string(err.what()));
    }
  }

  static void Render(Game* game, SDL_Renderer* renderer, SDL_Texture* gameTexture, float deltaTime);

 private:
  static void FPSWindow(float deltaTime);
  static void EntityInfoWindow(const Registry* registry);
};
#endif
