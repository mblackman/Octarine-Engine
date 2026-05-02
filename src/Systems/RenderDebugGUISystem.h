#pragma once

#include <SDL3/SDL.h>

#include "../General/Logger.h"
#include "Components/ScriptComponent.h"
#include "ECS/Iterable.h"
#include "ECS/Registry.h"
#include "Game/GameConfig.h"
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"

class RenderDebugGUISystem {
 public:
  void operator()(const ContextFacade& context, ScriptComponent& script) const {
    if (script.onDebugGUIFunction == sol::lua_nil) {
      return;
    }
    if (auto result = script.onDebugGUIFunction(script.scriptTable, context.Entity()); !result.valid()) {
      const sol::error err = result;
      Logger::ErrorLua(std::string(err.what()));
    }
  }

  static void Render(Registry* registry, SDL_Renderer* renderer, const float deltaTime) {
    auto& engineOptions = registry->Get<GameConfig>().GetEngineOptions();
    if (!engineOptions.showDebugGUI && !engineOptions.showFpsCounter) {
      return;
    }

    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    if (engineOptions.showFpsCounter) {
      FPSWindow(deltaTime);
    }
    if (engineOptions.showEntityInfo) {
      EntityInfoWindow(registry);
    }
    if (engineOptions.showDebugGUI) {
      EngineOptionsWindow(engineOptions);

      if (engineOptions.showImGuiDemoWindow) {
        ImGui::ShowDemoWindow();
      }

      auto query = registry->CreateQuery<ScriptComponent>();
      RenderDebugGUISystem system;
      query->ForEach(system);
    }

    ImGui::Render();
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
  }

 private:
  static void EngineOptionsWindow(EngineOptions& options) {
    ImGui::Begin("Engine Options");
    ImGui::Checkbox("Show ImGui Demo Window", &options.showImGuiDemoWindow);
    ImGui::Checkbox("Show FPS Counter", &options.showFpsCounter);
    ImGui::Checkbox("Show Entity Info", &options.showEntityInfo);
    ImGui::Checkbox("Draw Colliders", &options.drawColliders);
    ImGui::End();
  }

  static void FPSWindow(const float deltaTime) {
    ImGui::Begin("FPS");
    ImGui::Text("FPS: %.2f", 1.0f / deltaTime);
    ImGui::End();
  }

  static void EntityInfoWindow(const Registry* registry) {
    const auto count = registry->GetEntityCount();
    ImGui::Begin("Entity Info");
    ImGui::Text("Entity Count: %d", count);
    ImGui::End();
  }
};
