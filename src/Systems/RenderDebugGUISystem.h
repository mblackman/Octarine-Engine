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

  void Update(SDL_Renderer* renderer) const {
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
    ImGui::ShowDemoWindow();

    for (auto entity : GetEntities()) {
      if (auto& script = entity.GetComponent<ScriptComponent>(); script.onDebugGUIFunction != sol::lua_nil) {
        if (auto result = script.onDebugGUIFunction(script.scriptTable, entity); !result.valid()) {
          sol::error err = result;
          std::string what = err.what();
          Logger::ErrorLua(what);
        }
      }
    }
    ImGui::Render();
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
  }
};
