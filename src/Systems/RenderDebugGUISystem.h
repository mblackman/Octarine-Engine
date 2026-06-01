#pragma once

#include <SDL3/SDL.h>

#include "../General/Logger.h"
#include "Components/ScriptComponent.h"
#include "ECS/Iterable.h"
#include "ECS/Registry.h"
#include "Game/GameConfig.h"

#ifdef OCTARINE_WITH_IMGUI
#include "imgui.h"
#include "imgui_impl_sdlrenderer3.h"

#ifdef OCTARINE_WITH_EDITOR
#include "../Editor/EditorPersistence.h"
#include "../Editor/Fonts/Roboto_Medium.h"
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
  /// Baseline font size in pixels that matches the unscaled style metrics below.
  /// Font sizes larger/smaller than this scale all widget metrics proportionally
  /// via ImGuiStyle::ScaleAllSizes so the UI stays balanced at any zoom level.
  static constexpr float kBaselineFontSize = 16.0F;

  /// Apply one of the three built-in ImGui styles.  0 = Dark, 1 = Light, 2 = Classic.
  /// `fontSizePx` is the current font size; widget metrics are scaled relative to
  /// kBaselineFontSize. Always re-applies baseline metrics first so repeated calls
  /// do not compound.
  static void ApplyEditorStyle(const int styleIndex, const float fontSizePx = kBaselineFontSize) {
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

    const float scale = (fontSizePx > 0.0F) ? (fontSizePx / kBaselineFontSize) : 1.0F;
    if (scale != 1.0F) {
      style.ScaleAllSizes(scale);
    }
  }

  /// Rebuild the ImGui font atlas at `sizePx` using the bundled Roboto TTF.
  /// MUST be called outside an active ImGui frame (after ImGui::Render or before
  /// the next NewFrame). Also re-uploads the renderer's device texture.
  static void RebuildEditorFont(const float sizePx) {
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->Clear();

    ImFontConfig fontConfig;
    fontConfig.SizePixels = (sizePx > 0.0F) ? sizePx : kBaselineFontSize;
    fontConfig.OversampleH = 3;
    fontConfig.OversampleV = 3;
    fontConfig.PixelSnapH = false;
    // Tell ImGui not to free our static data — it lives in .rodata.
    fontConfig.FontDataOwnedByAtlas = false;

    ImFont* font = io.Fonts->AddFontFromMemoryTTF(
        const_cast<unsigned char*>(octarine::editor::fonts::kRobotoMediumData),
        static_cast<int>(octarine::editor::fonts::kRobotoMediumSize), fontConfig.SizePixels, &fontConfig);
    if (font == nullptr) {
      // Fallback so the editor still boots if the embedded load fails for any reason.
      io.FontDefault = io.Fonts->AddFontDefault();
    } else {
      io.FontDefault = font;
    }
    io.Fonts->Build();
    ImGui_ImplSDLRenderer3_DestroyDeviceObjects();
    ImGui_ImplSDLRenderer3_CreateDeviceObjects();
  }
#endif

  static void Render(Game* game, SDL_Renderer* renderer, SDL_Texture* gameTexture, const float deltaTime);

 private:
#ifdef OCTARINE_WITH_EDITOR
  static void SceneManagementWindow(Game* game);
  static void EngineOptionsWindow(EngineOptions& options, bool* p_open);
  static void EditorSettingsWindow(EditorPersistence& editorPersistence, bool* p_open);
  static void PerformanceProfilerWindow();
  static void HierarchyWindow(Registry* registry);
  static void ProjectSelectorWindow(Game* game, bool* p_open);
  static void SceneWindow(Game* game, SDL_Texture* gameTexture, bool* p_open);
  static void AssetBrowserWindow(Registry* registry);
  static void LuaConsoleWindow(sol::state& lua);
  static void PlayerOutputWindow(Game* game, bool* p_open);
  static void ExportOutputWindow(Game* game, bool* p_open);
  static void SigningSettingsWindow(bool* p_open);
#endif
  static void FPSWindow(float deltaTime);
  static void EntityInfoWindow(const Registry* registry);
};
#endif
