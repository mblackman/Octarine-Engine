#pragma once

#include <SDL3/SDL.h>

#include "../AssetManager/AssetManager.h"
#include "../General/Logger.h"
#include "../General/PerfUtils.h"
#include "Components/AnimationComponent.h"
#include "Components/BoxColliderComponent.h"
#include "Components/CameraFollowComponent.h"
#include "Components/EntityMaskComponent.h"
#include "Components/HealthComponent.h"
#include "Components/KeyboardControlComponent.h"
#include "Components/ProjectileComponent.h"
#include "Components/ProjectileEmitterComponent.h"
#include "Components/RigidBodyComponent.h"
#include "Components/ScriptComponent.h"
#include "Components/SpriteComponent.h"
#include "Components/SquarePrimitiveComponent.h"
#include "Components/TextLabelComponent.h"
#include "Components/TransformComponent.h"
#include "Components/UIButtonComponent.h"
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

  static void Render(Registry* registry, SDL_Renderer* renderer, SDL_Texture* gameTexture, sol::state& lua,
                     const float deltaTime) {
    auto& gameConfig = registry->Get<GameConfig>();
    auto& engineOptions = gameConfig.GetEngineOptions();
    const bool projectLoaded = gameConfig.HasLoadedConfig();
    const bool editorSession = gameConfig.IsEditorMode() || !projectLoaded;

    // Visibility logic:
    // 1. Editor windows: Always show in Editor Mode, never in Play Mode.
    // 2. Game overlays (Lua UI, FPS, Entity Info): Toggle with showDebugGUI in both modes?
    //    User said: "When in play mode, use the grave key to toggle game debug menus... fps counter... entity counter"
    //    In editor mode, we probably want them as windows or toggles too.

    const bool showEditorUI = editorSession;                   // Editor windows always show in Editor session
    const bool showGameOverlays = engineOptions.showDebugGUI;  // Toggled by Grave key

    if (!showEditorUI && !showGameOverlays && !engineOptions.showFpsCounter) {
      // Small optimization: FPS counter can be on without showDebugGUI if user manually set it,
      // but usually showDebugGUI toggles it.
      return;
    }

    PROFILE_NAMED_SCOPE("RenderDebugGUISystem::Render");

    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    static bool showProjectSelector = false;

    if (showEditorUI) {
      ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0));
      ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());
      ImGui::PopStyleColor();

      if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
          if (ImGui::MenuItem("Open Project...", "Ctrl+O")) {
            showProjectSelector = true;
          }
          if (ImGui::MenuItem("Save Preferences", "Ctrl+S")) {
            gameConfig.SaveUserPreferences();
            gameConfig.SaveGlobalPreferences();
          }
          ImGui::Separator();
          if (ImGui::MenuItem("Quit", "Alt+F4")) {
            Game::Quit();
          }
          ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Windows")) {
          ImGui::MenuItem("Scene", nullptr, nullptr, false);
          ImGui::MenuItem("Hierarchy", nullptr, &engineOptions.showHierarchy);
          ImGui::MenuItem("Asset Browser", nullptr, &engineOptions.showAssetBrowser);
          ImGui::MenuItem("Lua Console", nullptr, &engineOptions.showLuaConsole);
          ImGui::MenuItem("Performance Profiler", nullptr, &engineOptions.showProfiler);
          ImGui::MenuItem("Game Debug Overlays", "Grave", &engineOptions.showDebugGUI);
          ImGui::Separator();
          ImGui::MenuItem("FPS Counter", nullptr, &engineOptions.showFpsCounter);
          ImGui::MenuItem("ImGui Demo Window", nullptr, &engineOptions.showImGuiDemoWindow);
          ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
      }

      SceneWindow(gameTexture);

      // --- Editor Mode Windows ---
      if (engineOptions.showHierarchy) HierarchyWindow(registry);
      if (engineOptions.showAssetBrowser) AssetBrowserWindow(registry);
      if (engineOptions.showLuaConsole) LuaConsoleWindow(lua);
      if (engineOptions.showProfiler) PerformanceProfilerWindow();
      EngineOptionsWindow(engineOptions);
      EditorSettingsWindow(engineOptions);

      if (engineOptions.showImGuiDemoWindow) {
        ImGui::ShowDemoWindow();
      }
    }

    // --- Game Overlays (Lua UI, FPS, Entity Info) ---
    // User requested these be toggled by the Grave key in Play mode.
    // In Editor mode, they can also be shown if toggled.
    if (showGameOverlays) {
      if (projectLoaded) {
        auto query = registry->CreateQuery<ScriptComponent>();
        RenderDebugGUISystem system;
        query->ForEach(system);
      }
      EntityInfoWindow(registry);
      FPSWindow(deltaTime);
    } else if (showEditorUI && engineOptions.showFpsCounter) {
      // In editor mode, if showDebugGUI is OFF, we might still want FPS if its specific flag is ON
      FPSWindow(deltaTime);
    }

    if (!projectLoaded || showProjectSelector) {
      ProjectSelectorWindow(registry, &showProjectSelector);
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
    ImGui::Checkbox("Show Lua Console", &options.showLuaConsole);
    ImGui::Checkbox("Draw Colliders", &options.drawColliders);
    ImGui::Checkbox("Audio Enabled", &options.audioEnabled);
    ImGui::SliderFloat("Master Volume", &options.masterVolume, 0.0F, 1.0F);
    ImGui::Separator();
    ImGui::Checkbox("Pause Execution", &options.isPaused);
    ImGui::SliderFloat("Time Scale", &options.timeScale, 0.0F, 5.0F);
    ImGui::End();
  }

  static void EditorSettingsWindow(EngineOptions& options) {
    ImGui::Begin("Editor Settings");

    // --- Font Size ---
    ImGui::SeparatorText("Appearance");
    static float pendingFontSize = options.editorFontSize;
    // Sync on first frame or if something else changed the value.
    if (pendingFontSize <= 0.0F) pendingFontSize = options.editorFontSize;

    ImGui::SliderFloat("Font Size", &pendingFontSize, 10.0F, 40.0F, "%.0f px");
    ImGui::SameLine();
    if (ImGui::Button("Reset")) {
      pendingFontSize = 0.0F;  // signals auto-detect on next apply
    }

    if (ImGui::Button("Apply Font Size")) {
      options.editorFontSize = pendingFontSize;
      // Rebuild the font atlas at the new size.
      ImGuiIO& io = ImGui::GetIO();
      io.Fonts->Clear();
      io.Fonts->AddFontDefault();
      ImFontConfig fontConfig;
      fontConfig.SizePixels = (pendingFontSize > 0.0F) ? pendingFontSize : 16.0F;
      fontConfig.OversampleH = 2;
      fontConfig.OversampleV = 2;
      io.FontDefault = io.Fonts->AddFontDefault(&fontConfig);
      io.Fonts->Build();
      // Force the SDL renderer backend to re-upload the new font atlas texture.
      ImGui_ImplSDLRenderer3_DestroyDeviceObjects();
      ImGui_ImplSDLRenderer3_CreateDeviceObjects();
    }
    ImGui::SameLine();
    ImGui::TextDisabled("(rebuilds font atlas)");

    // --- Theme ---
    const char* styles[] = {"Dark", "Light", "Classic"};
    if (ImGui::Combo("Theme", &options.editorStyleIndex, styles, IM_ARRAYSIZE(styles))) {
      ApplyEditorStyle(options.editorStyleIndex);
    }

    ImGui::End();
  }

  static void FPSWindow(const float deltaTime) {
    static float fpsHistory[120] = {};
    static int fpsOffset = 0;
    const float fps = (deltaTime > 0.0f) ? 1.0f / deltaTime : 0.0f;
    fpsHistory[fpsOffset] = fps;
    fpsOffset = (fpsOffset + 1) % 120;

    ImGui::Begin("FPS");
    char overlay[32];
    snprintf(overlay, sizeof(overlay), "%.1f FPS", fps);
    ImGui::PlotLines("##fps", fpsHistory, 120, fpsOffset, overlay, 0.0f, 120.0f, ImVec2(0, 50));
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

    ImGui::Separator();
    const auto counters = PerfUtils::PerfCounters::GetCounters();
    if (!counters.empty() && ImGui::BeginTable("counters_table", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
      ImGui::TableSetupColumn("Counter");
      ImGui::TableSetupColumn("Value");
      ImGui::TableHeadersRow();

      for (const auto& [name, value] : counters) {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("%s", name.c_str());
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%lld", value);
      }
      ImGui::EndTable();
    }
    ImGui::End();
  }

  static void HierarchyWindow(Registry* registry) {
    ImGui::Begin("Hierarchy / Entity Inspector");
    const auto entities = registry->GetUserEntities();
    ImGui::Text("Total User Entities: %zu", entities.size());
    if (ImGui::Button("Create Entity")) {
      registry->CreateEntity();
    }

    static Entity selectedEntity{UINT64_MAX};
    static ImGuiTextFilter entityFilter;
    entityFilter.Draw("##filter", 140);

    ImGui::BeginChild("EntityList", ImVec2(150, 0), true);
    for (const auto& entity : entities) {
      std::string label = "Entity " + std::to_string(entity.id);
      if (!entityFilter.PassFilter(label.c_str())) continue;
      if (ImGui::Selectable(label.c_str(), selectedEntity == entity)) {
        selectedEntity = entity;
      }
    }
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("EntityDetails", ImVec2(0, 0), true);
    if (registry->IsAlive(selectedEntity)) {
      ImGui::Text("Selected Entity ID: %u", selectedEntity.GetId());
      ImGui::SameLine();
      if (ImGui::Button("Destroy Entity")) {
        registry->BlamEntity(selectedEntity);
        selectedEntity = Entity{UINT64_MAX};
      }
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
          ImGui::DragInt("Layer", &sprite.layer);
          ImGui::Text("Asset ID: %s", sprite.assetId.c_str());
          ImGui::DragFloat("Width", &sprite.width);
          ImGui::DragFloat("Height", &sprite.height);
          ImGui::Checkbox("Fixed", &sprite.isFixed);
          // Sprite texture preview
          auto& assetManager = registry->Get<AssetManager>();
          if (auto* tex = assetManager.GetTexture(sprite.assetId); tex != nullptr) {
            ImGui::Image(reinterpret_cast<ImTextureID>(tex), ImVec2(64, 64));
          }
        }
      }
      if (registry->HasComponent<RigidBodyComponent>(selectedEntity)) {
        if (ImGui::CollapsingHeader("RigidBody", ImGuiTreeNodeFlags_DefaultOpen)) {
          auto& rb = registry->GetComponent<RigidBodyComponent>(selectedEntity);
          ImGui::DragFloat2("Velocity", &rb.velocity.x, 1.0f);
        }
      }
      if (registry->HasComponent<BoxColliderComponent>(selectedEntity)) {
        if (ImGui::CollapsingHeader("BoxCollider", ImGuiTreeNodeFlags_DefaultOpen)) {
          auto& bc = registry->GetComponent<BoxColliderComponent>(selectedEntity);
          ImGui::DragInt("Width", &bc.width);
          ImGui::DragInt("Height", &bc.height);
          ImGui::DragFloat2("Offset", &bc.offset.x, 1.0f);
        }
      }
      if (registry->HasComponent<AnimationComponent>(selectedEntity)) {
        if (ImGui::CollapsingHeader("Animation", ImGuiTreeNodeFlags_DefaultOpen)) {
          auto& anim = registry->GetComponent<AnimationComponent>(selectedEntity);
          ImGui::Text("Num Frames: %d", anim.numFrames);
          ImGui::SliderInt("Current Frame", &anim.currentFrame, 1, anim.numFrames);
          ImGui::DragInt("Frame Speed (ms)", &anim.frameRateSpeed);
          ImGui::Checkbox("Looping", &anim.shouldLoop);
        }
      }
      if (registry->HasComponent<ProjectileEmitterComponent>(selectedEntity)) {
        if (ImGui::CollapsingHeader("ProjectileEmitter", ImGuiTreeNodeFlags_DefaultOpen)) {
          auto& pe = registry->GetComponent<ProjectileEmitterComponent>(selectedEntity);
          ImGui::DragFloat2("Velocity", &pe.velocity.x, 1.0f);
          ImGui::DragFloat("Frequency (s)", &pe.frequency, 0.1f);
          ImGui::DragFloat("Duration (s)", &pe.duration, 0.1f);
          ImGui::DragInt("Damage", &pe.damage);
        }
      }
      if (registry->HasComponent<TextLabelComponent>(selectedEntity)) {
        if (ImGui::CollapsingHeader("TextLabel", ImGuiTreeNodeFlags_DefaultOpen)) {
          auto& tl = registry->GetComponent<TextLabelComponent>(selectedEntity);
          ImGui::Text("Text: %s", tl.text.c_str());
          ImGui::Text("Font ID: %s", tl.fontId.c_str());
          float col[4] = {tl.color.r / 255.0f, tl.color.g / 255.0f, tl.color.b / 255.0f, tl.color.a / 255.0f};
          if (ImGui::ColorEdit4("Color", col)) {
            tl.color = {static_cast<Uint8>(col[0] * 255), static_cast<Uint8>(col[1] * 255),
                        static_cast<Uint8>(col[2] * 255), static_cast<Uint8>(col[3] * 255)};
          }
          ImGui::Checkbox("Fixed", &tl.isFixed);
        }
      }
      if (registry->HasComponent<ScriptComponent>(selectedEntity)) {
        if (ImGui::CollapsingHeader("Script", ImGuiTreeNodeFlags_DefaultOpen)) {
          ImGui::Text("Has Lua script table and callbacks.");
        }
      }
      if (registry->HasComponent<CameraFollowComponent>(selectedEntity)) {
        if (ImGui::CollapsingHeader("CameraFollow", ImGuiTreeNodeFlags_DefaultOpen)) {
          ImGui::Text("Entity is being followed by the camera.");
        }
      }
      if (registry->HasComponent<EntityMaskComponent>(selectedEntity)) {
        if (ImGui::CollapsingHeader("EntityMask", ImGuiTreeNodeFlags_DefaultOpen)) {
          auto& em = registry->GetComponent<EntityMaskComponent>(selectedEntity);
          ImGui::Text("Mask: %s", em.mask.to_string().c_str());
        }
      }
      if (registry->HasComponent<KeyboardControlComponent>(selectedEntity)) {
        if (ImGui::CollapsingHeader("KeyboardControl", ImGuiTreeNodeFlags_DefaultOpen)) {
          auto& kc = registry->GetComponent<KeyboardControlComponent>(selectedEntity);
          auto vel = static_cast<float>(kc.velocity);
          if (ImGui::DragFloat("Velocity", &vel)) {
            kc.velocity = vel;
          }
        }
      }
      if (registry->HasComponent<ProjectileComponent>(selectedEntity)) {
        if (ImGui::CollapsingHeader("Projectile", ImGuiTreeNodeFlags_DefaultOpen)) {
          auto& pc = registry->GetComponent<ProjectileComponent>(selectedEntity);
          ImGui::DragInt("Damage", &pc.damage);
          ImGui::Text("Timer: %.2f / %.2f", pc.timer, pc.duration);
        }
      }
      if (registry->HasComponent<SquarePrimitiveComponent>(selectedEntity)) {
        if (ImGui::CollapsingHeader("SquarePrimitive", ImGuiTreeNodeFlags_DefaultOpen)) {
          auto& sp = registry->GetComponent<SquarePrimitiveComponent>(selectedEntity);
          ImGui::DragFloat2("Position", &sp.position.x);
          ImGui::DragInt("Layer", &sp.layer);
          ImGui::DragFloat("Width", &sp.width);
          ImGui::DragFloat("Height", &sp.height);
          float col[4] = {sp.color.r / 255.0f, sp.color.g / 255.0f, sp.color.b / 255.0f, sp.color.a / 255.0f};
          if (ImGui::ColorEdit4("Color", col)) {
            sp.color = {static_cast<Uint8>(col[0] * 255), static_cast<Uint8>(col[1] * 255),
                        static_cast<Uint8>(col[2] * 255), static_cast<Uint8>(col[3] * 255)};
          }
          ImGui::Checkbox("Fixed", &sp.isFixed);
        }
      }
      if (registry->HasComponent<UIButtonComponent>(selectedEntity)) {
        if (ImGui::CollapsingHeader("UIButton", ImGuiTreeNodeFlags_DefaultOpen)) {
          auto& ub = registry->GetComponent<UIButtonComponent>(selectedEntity);
          ImGui::Checkbox("Active", &ub.isActive);
        }
      }

      // --- Add Component dropdown ---
      ImGui::Separator();
      if (ImGui::BeginCombo("Add Component", "Select...")) {
        if (!registry->HasComponent<TransformComponent>(selectedEntity) && ImGui::Selectable("Transform")) {
          registry->AddComponent(selectedEntity, TransformComponent());
        }
        if (!registry->HasComponent<RigidBodyComponent>(selectedEntity) && ImGui::Selectable("RigidBody")) {
          registry->AddComponent(selectedEntity, RigidBodyComponent());
        }
        if (!registry->HasComponent<SpriteComponent>(selectedEntity) && ImGui::Selectable("Sprite")) {
          registry->AddComponent(selectedEntity, SpriteComponent());
        }
        if (!registry->HasComponent<BoxColliderComponent>(selectedEntity) && ImGui::Selectable("BoxCollider")) {
          registry->AddComponent(selectedEntity, BoxColliderComponent());
        }
        if (!registry->HasComponent<AnimationComponent>(selectedEntity) && ImGui::Selectable("Animation")) {
          registry->AddComponent(selectedEntity, AnimationComponent());
        }
        if (!registry->HasComponent<HealthComponent>(selectedEntity) && ImGui::Selectable("Health")) {
          registry->AddComponent(selectedEntity, HealthComponent(100));
        }
        if (!registry->HasComponent<KeyboardControlComponent>(selectedEntity) && ImGui::Selectable("KeyboardControl")) {
          registry->AddComponent(selectedEntity, KeyboardControlComponent());
        }
        if (!registry->HasComponent<CameraFollowComponent>(selectedEntity) && ImGui::Selectable("CameraFollow")) {
          registry->AddComponent(selectedEntity, CameraFollowComponent());
        }
        ImGui::EndCombo();
      }
    } else {
      ImGui::Text("No entity selected or entity destroyed.");
    }
    ImGui::EndChild();

    ImGui::End();
  }

  static void ProjectSelectorWindow(Registry* registry, bool* p_open) {
    ImGui::SetNextWindowSize(ImVec2(400, 200), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin("Project Selector", p_open, ImGuiWindowFlags_NoDocking)) {
      ImGui::End();
      return;
    }

    auto& gameConfig = registry->Get<GameConfig>();
    auto& options = gameConfig.GetEngineOptions();

    static char projectPath[256] = "";
    if (projectPath[0] == '\0' && !options.lastProjectPath.empty()) {
      strncpy(projectPath, options.lastProjectPath.c_str(), sizeof(projectPath));
    }

    ImGui::Text("Enter Project Path:");
    ImGui::InputText("##path", projectPath, sizeof(projectPath));

    if (ImGui::Button("Open Project")) {
      Logger::Info("Opening project: " + std::string(projectPath));
      // For now, we'll just set lastProjectPath and tell the user to restart,
      // or we can try to implement a reload.
      options.lastProjectPath = projectPath;
      gameConfig.SaveGlobalPreferences();
      *p_open = false;

      // Hint: To truly switch projects at runtime, we'd need to clear the registry,
      // reload assets, and re-run Setup().
      ImGui::OpenPopup("Restart Required");
    }

    if (ImGui::BeginPopupModal("Restart Required", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
      ImGui::Text("Project path saved. Please restart the engine to load the new project.");
      if (ImGui::Button("OK", ImVec2(120, 0))) {
        ImGui::CloseCurrentPopup();
      }
      ImGui::EndPopup();
    }

    if (!options.lastProjectPath.empty()) {
      ImGui::Separator();
      ImGui::Text("Last Project: %s", options.lastProjectPath.c_str());
      if (ImGui::Button("Load Last Project")) {
        strncpy(projectPath, options.lastProjectPath.c_str(), sizeof(projectPath));
      }
    }

    ImGui::End();
  }

  static void SceneWindow(SDL_Texture* gameTexture) {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::Begin("Scene");
    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    if (gameTexture) {
      float texW = 0, texH = 0;
      SDL_GetTextureSize(gameTexture, &texW, &texH);

      if (texW > 0 && texH > 0) {
        float aspectRatio = texW / texH;
        float targetW = viewportPanelSize.x;
        float targetH = targetW / aspectRatio;

        if (targetH > viewportPanelSize.y) {
          targetH = viewportPanelSize.y;
          targetW = targetH * aspectRatio;
        }

        ImVec2 imageSize(targetW, targetH);
        ImVec2 imagePos =
            ImVec2((viewportPanelSize.x - imageSize.x) * 0.5f, (viewportPanelSize.y - imageSize.y) * 0.5f);

        ImGui::SetCursorPos(imagePos);
        ImGui::Image(reinterpret_cast<ImTextureID>(gameTexture), imageSize);
      }
    }
    ImGui::End();
    ImGui::PopStyleVar();
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

  static void LuaConsoleWindow(sol::state& lua) {
    static char inputBuffer[256] = "";
    static std::vector<std::string> commandHistory;
    static int commandHistoryIndex = -1;
    static bool scrollToBottom = false;
    static bool reclaimFocus = false;

    ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin("Lua Console")) {
      ImGui::End();
      return;
    }

    if (ImGui::Button("Clear")) {
      Logger::ClearHistory();
    }
    ImGui::SameLine();
    ImGui::TextDisabled("Type Lua commands below. Use Up/Down for history.");

    // History area
    const float footerHeightToReserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
    ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footerHeightToReserve), false,
                      ImGuiWindowFlags_HorizontalScrollbar);

    auto logHistory = Logger::GetHistory();
    for (const auto& line : logHistory) {
      ImVec4 color = ImVec4(1.0F, 1.0F, 1.0F, 1.0F);
      if (line.find("[Error]") != std::string::npos) {
        color = ImVec4(1.0F, 0.4F, 0.4F, 1.0F);
      } else if (line.find("[Warn]") != std::string::npos) {
        color = ImVec4(1.0F, 1.0F, 0.4F, 1.0F);
      } else if (line.find("[Info]") != std::string::npos) {
        color = ImVec4(0.4F, 1.0F, 0.4F, 1.0F);
      } else if (line.find("> ") == 0) {
        color = ImVec4(0.4F, 0.8F, 1.0F, 1.0F);
      }

      ImGui::PushStyleColor(ImGuiCol_Text, color);
      ImGui::TextUnformatted(line.c_str());
      ImGui::PopStyleColor();
    }

    if (scrollToBottom) {
      ImGui::SetScrollHereY(1.0f);
    }
    scrollToBottom = false;
    ImGui::EndChild();

    ImGui::Separator();

    // Input area
    struct CallbackData {
      std::vector<std::string>* history;
      int* index;
    } cbData{&commandHistory, &commandHistoryIndex};

    auto wrappedCallback = [](ImGuiInputTextCallbackData* data) -> int {
      auto* cb = static_cast<CallbackData*>(data->UserData);
      if (data->EventFlag == ImGuiInputTextFlags_CallbackHistory) {
        if (data->EventKey == ImGuiKey_UpArrow) {
          if (*cb->index == -1) {
            *cb->index = static_cast<int>(cb->history->size()) - 1;
          } else if (*cb->index > 0) {
            (*cb->index)--;
          }
        } else if (data->EventKey == ImGuiKey_DownArrow) {
          if (*cb->index != -1) {
            if (*cb->index < static_cast<int>(cb->history->size()) - 1) {
              (*cb->index)++;
            } else {
              *cb->index = -1;
            }
          }
        }
        if (*cb->index != -1) {
          data->DeleteChars(0, data->BufTextLen);
          data->InsertChars(0, (*cb->history)[*cb->index].c_str());
        } else {
          data->DeleteChars(0, data->BufTextLen);
        }
      }
      return 0;
    };

    ImGuiInputTextFlags inputTextFlags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackHistory;
    if (ImGui::InputText("##Input", inputBuffer, IM_ARRAYSIZE(inputBuffer), inputTextFlags, wrappedCallback, &cbData)) {
      std::string command(inputBuffer);
      if (!command.empty()) {
        Logger::LogLua("> " + command);
        commandHistory.push_back(command);
        commandHistoryIndex = -1;

        auto result = lua.safe_script(command, sol::script_pass_on_error);
        if (!result.valid()) {
          const sol::error err = result;
          Logger::ErrorLua(std::string(err.what()));
        }

        inputBuffer[0] = '\0';
        scrollToBottom = true;
        reclaimFocus = true;
      }
    }

    // Auto-focus
    if (reclaimFocus) {
      ImGui::SetKeyboardFocusHere(-1);
      reclaimFocus = false;
    } else if (ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) && !ImGui::IsAnyItemActive() &&
               !ImGui::IsMouseClicked(0)) {
      ImGui::SetKeyboardFocusHere(-1);
    }

    ImGui::End();
  }
};
