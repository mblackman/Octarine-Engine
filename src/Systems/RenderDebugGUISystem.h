#pragma once

#include <SDL3/SDL.h>

#include "../General/Logger.h"
#include "Components/ScriptComponent.h"
#include "ECS/Iterable.h"
#include "ECS/Registry.h"
#include "Game/GameConfig.h"
#include "imgui.h"

class Game;

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

  /// Apply one of the three built-in ImGui styles.  0 = Dark, 1 = Light, 2 = Classic.
  static void ApplyEditorStyle(const int styleIndex) {
    switch (styleIndex) {
      default:
      case 0:
        ImGui::StyleColorsDark();
        break;
      case 1:
        ImGui::StyleColorsLight();
        break;
      case 2:
        ImGui::StyleColorsClassic();
        break;
    }
    // Slightly rounded corners and extra frame padding for readability at any scale.
    ImGuiStyle& style = ImGui::GetStyle();
    style.FrameRounding = 4.0F;
    style.GrabRounding = 4.0F;
    style.WindowRounding = 6.0F;
    style.FramePadding = ImVec2(6, 4);
    style.ItemSpacing = ImVec2(8, 6);
    style.ScrollbarSize = 16.0F;
    style.GrabMinSize = 14.0F;
  }

  static void Render(Game* game, SDL_Renderer* renderer, SDL_Texture* gameTexture, const float deltaTime);

 private:
  static void SceneManagementWindow(Game* game);
  static void EngineOptionsWindow(EngineOptions& options);
  static void EditorSettingsWindow(EngineOptions& options);
  static void FPSWindow(float deltaTime);
  static void EntityInfoWindow(const Registry* registry);
  static void PerformanceProfilerWindow();
  static void HierarchyWindow(Registry* registry);
  static void ProjectSelectorWindow(Game* game, bool* p_open);
  static void SceneWindow(SDL_Texture* gameTexture, bool* p_open);
  static void AssetBrowserWindow(Registry* registry);
  static void LuaConsoleWindow(sol::state& lua);
};
