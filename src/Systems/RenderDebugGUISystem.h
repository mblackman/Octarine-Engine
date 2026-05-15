#pragma once

#include <SDL3/SDL.h>

#include "../General/Logger.h"
#include "Components/ScriptComponent.h"
#include "ECS/Iterable.h"
#include "ECS/Registry.h"
#include "Game/GameConfig.h"

#ifdef OCTARINE_WITH_IMGUI
#include "imgui.h"

#ifdef OCTARINE_WITH_EDITOR
#include "../Editor/EditorPersistence.h"
#endif

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

#ifdef OCTARINE_WITH_EDITOR
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
    ImGuiStyle& style = ImGui::GetStyle();
    style.FrameRounding = 4.0F;
    style.GrabRounding = 4.0F;
    style.WindowRounding = 6.0F;
    style.FramePadding = ImVec2(6, 4);
    style.ItemSpacing = ImVec2(8, 6);
    style.ScrollbarSize = 16.0F;
    style.GrabMinSize = 14.0F;
  }
#endif

  static void Render(Game* game, SDL_Renderer* renderer, SDL_Texture* gameTexture, const float deltaTime);

 private:
#ifdef OCTARINE_WITH_EDITOR
  static void SceneManagementWindow(Game* game);
  static void EngineOptionsWindow(EngineOptions& options, EditorPersistence& editorPersistence);
  static void EditorSettingsWindow(EditorPersistence& editorPersistence);
  static void PerformanceProfilerWindow();
  static void HierarchyWindow(Registry* registry);
  static void ProjectSelectorWindow(Game* game, bool* p_open);
  static void SceneWindow(SDL_Texture* gameTexture, bool* p_open);
  static void AssetBrowserWindow(Registry* registry);
  static void LuaConsoleWindow(sol::state& lua);
#endif
  static void FPSWindow(float deltaTime);
  static void EntityInfoWindow(const Registry* registry);
};
#endif
