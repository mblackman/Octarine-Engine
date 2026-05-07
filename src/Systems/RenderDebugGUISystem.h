#pragma once

#include <SDL3/SDL.h>

#include "../AssetManager/AssetManager.h"
#include "../General/Logger.h"
#include "../General/PerfUtils.h"
#include "Components/HealthComponent.h"
#include "Components/ScriptComponent.h"
#include "Components/SpriteComponent.h"
#include "Components/TransformComponent.h"
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
    if (auto result = script.onDebugGUIFunction(script.scriptTable, context.GetEntity()); !result.valid()) {
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
    if (engineOptions.showProfiler) {
      PerformanceProfilerWindow();
    }
    if (engineOptions.showHierarchy) {
      HierarchyWindow(registry);
    }
    if (engineOptions.showAssetBrowser) {
      AssetBrowserWindow(registry);
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
    ImGui::Checkbox("Show Profiler", &options.showProfiler);
    ImGui::Checkbox("Show Hierarchy", &options.showHierarchy);
    ImGui::Checkbox("Show Asset Browser", &options.showAssetBrowser);
    ImGui::Checkbox("Draw Colliders", &options.drawColliders);
    ImGui::Checkbox("Audio Enabled", &options.audioEnabled);
    ImGui::SliderFloat("Master Volume", &options.masterVolume, 0.0F, 1.0F);
    ImGui::Separator();
    ImGui::Checkbox("Pause Execution", &options.isPaused);
    ImGui::SliderFloat("Time Scale", &options.timeScale, 0.0F, 5.0F);
    ImGui::End();
  }

  static void FPSWindow(const float deltaTime) {
    ImGui::Begin("FPS");
    ImGui::Text("FPS: %.2f", 1.0f / deltaTime);
    ImGui::End();
  }

  static void EntityInfoWindow(const Registry* registry) {
    const auto count = registry->GetUserEntityCount();
    ImGui::Begin("Entity Info");
    ImGui::Text("Entity Count: %llu", static_cast<unsigned long long>(count));
    ImGui::End();
  }

  static void PerformanceProfilerWindow() {
    ImGui::Begin("Performance Profiler");
    const auto times = PerfUtils::ProfilingAccumulator::GetAccumulatedTimes();
    if (ImGui::BeginTable("profiler_table", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
      ImGui::TableSetupColumn("System/Scope");
      ImGui::TableSetupColumn("Time (ms)");
      ImGui::TableHeadersRow();

      for (const auto& [name, duration_us] : times) {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("%s", name.c_str());
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%.3f", static_cast<double>(duration_us) * 0.001);
      }
      ImGui::EndTable();
    }
    ImGui::End();
  }

  static void HierarchyWindow(Registry* registry) {
    ImGui::Begin("Hierarchy / Entity Inspector");
    const auto entities = registry->GetUserEntities();
    ImGui::Text("Total User Entities: %zu", entities.size());

    static Entity selectedEntity;

    ImGui::BeginChild("EntityList", ImVec2(150, 0), true);
    for (const auto& entity : entities) {
      std::string label = "Entity " + std::to_string(entity.id);
      if (ImGui::Selectable(label.c_str(), selectedEntity == entity)) {
        selectedEntity = entity;
      }
    }
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("EntityDetails", ImVec2(0, 0), true);
    if (registry->IsAlive(selectedEntity)) {
      ImGui::Text("Selected Entity ID: %u", selectedEntity.GetId());
      ImGui::Separator();

      if (registry->HasComponent<TransformComponent>(selectedEntity)) {
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
          auto& transform = registry->GetComponent<TransformComponent>(selectedEntity);
          ImGui::DragFloat2("Position", &transform.position.x);
          ImGui::DragFloat2("Scale", &transform.scale.x);
          auto rotation = static_cast<float>(transform.rotation);
          if (ImGui::DragFloat("Rotation", &rotation)) {
            transform.rotation = rotation;
          }
        }
      }
      if (registry->HasComponent<HealthComponent>(selectedEntity)) {
        if (ImGui::CollapsingHeader("Health", ImGuiTreeNodeFlags_DefaultOpen)) {
          auto& health = registry->GetComponent<HealthComponent>(selectedEntity);
          ImGui::SliderInt("HP", &health.currentHealth, 0, health.maxHealth);
        }
      }
      if (registry->HasComponent<SpriteComponent>(selectedEntity)) {
        if (ImGui::CollapsingHeader("Sprite", ImGuiTreeNodeFlags_DefaultOpen)) {
          auto& sprite = registry->GetComponent<SpriteComponent>(selectedEntity);
          ImGui::Text("Layer: %d", sprite.layer);
        }
      }
    } else {
      ImGui::Text("No entity selected or entity destroyed.");
    }
    ImGui::EndChild();

    ImGui::End();
  }

  static void AssetBrowserWindow(Registry* registry) {
    ImGui::Begin("Asset Browser");
    auto& assetManager = registry->Get<AssetManager>();

    if (ImGui::CollapsingHeader("Textures", ImGuiTreeNodeFlags_DefaultOpen)) {
      const auto& textures = assetManager.GetTextures();
      ImGui::Text("Loaded: %zu", textures.size());
      for (const auto& [id, texture] : textures) {
        ImGui::BulletText("%s", id.c_str());
      }
    }

    if (ImGui::CollapsingHeader("Fonts", ImGuiTreeNodeFlags_DefaultOpen)) {
      const auto& fonts = assetManager.GetFonts();
      ImGui::Text("Loaded: %zu", fonts.size());
      for (const auto& [id, font] : fonts) {
        ImGui::BulletText("%s", id.c_str());
      }
    }

    if (ImGui::CollapsingHeader("Audio Clips", ImGuiTreeNodeFlags_DefaultOpen)) {
      const auto& audioClips = assetManager.GetAudioClips();
      ImGui::Text("Loaded: %zu", audioClips.size());
      for (const auto& [id, clip] : audioClips) {
        ImGui::BulletText("%s", id.c_str());
      }
    }

    ImGui::End();
  }
};
