#pragma once

#include <SDL3/SDL.h>

#include "../ECS/ECS.h"
#include "Components/ScriptComponent.h"
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"

class RenderDebugGUISystem : public System {
 public:
  RenderDebugGUISystem() { RequireComponent<ScriptComponent>(); }

  RenderDebugGUISystem(const RenderDebugGUISystem&) = delete;
  RenderDebugGUISystem& operator=(const RenderDebugGUISystem&) = delete;

  RenderDebugGUISystem(RenderDebugGUISystem&&) = delete;
  RenderDebugGUISystem& operator=(RenderDebugGUISystem&&) = delete;

  ~RenderDebugGUISystem() = default;

  void Update(const float deltaTime, SDL_Renderer* renderer) const {
    if (!GameConfig::GetInstance().GetEngineOptions().showDebugGUI &&
        !GameConfig::GetInstance().GetEngineOptions().showFpsCounter) {
      return;
    }

    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    if (GameConfig::GetInstance().GetEngineOptions().showFpsCounter) {
      FPSWindow(deltaTime);
    }
    if (GameConfig::GetInstance().GetEngineOptions().showDebugGUI) {
      EngineOptionsWindow();

      if (GameConfig::GetInstance().GetEngineOptions().showImGuiDemoWindow) {
        ImGui::ShowDemoWindow();
      }

      for (auto entity : GetEntities()) {
        if (auto& script = entity.GetComponent<ScriptComponent>(); script.onDebugGUIFunction != sol::lua_nil) {
          if (auto result = script.onDebugGUIFunction(script.scriptTable, entity); !result.valid()) {
            sol::error err = result;
            std::string what = err.what();
            Logger::ErrorLua(what);
          }
        }
      }
    }

    ImGui::Render();
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
  }

 private:
  void EngineOptionsWindow() const {
    ImGui::Begin("Engine Options");
    ImGui::Checkbox("Show ImGui Demo Window", &GameConfig::GetInstance().GetEngineOptions().showImGuiDemoWindow);
    ImGui::Checkbox("Show FPS Counter", &GameConfig::GetInstance().GetEngineOptions().showFpsCounter);
    ImGui::Checkbox("Draw Colliders", &GameConfig::GetInstance().GetEngineOptions().drawColliders);
    ImGui::End();
  }

  void FPSWindow(const float deltaTime) const {
    ImGui::Begin("FPS");
    ImGui::Text("FPS: %.2f", 1.0f / deltaTime);
    ImGui::End();
  }
};
