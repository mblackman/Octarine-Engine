#include "RenderDebugGUISystem.h"

#ifdef OCTARINE_WITH_IMGUI
#include <filesystem>

#include "../AssetManager/AssetManager.h"
#include "../Components/AnimationComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/CameraFollowComponent.h"
#include "../Components/EntityMaskComponent.h"
#include "../Components/HealthComponent.h"
#include "../Components/KeyboardControlComponent.h"
#include "../Components/ProjectileComponent.h"
#include "../Components/ProjectileEmitterComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/SquarePrimitiveComponent.h"
#include "../Components/TextLabelComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/UIButtonComponent.h"
#include "../Game/Game.h"
#include "../Game/GameConfig.h"
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"

namespace {
#ifdef OCTARINE_WITH_EDITOR
void SDLCALL OnProjectFolderSelected(void* userdata, const char* const* filelist, int /*filter*/) {
  auto* game = static_cast<Game*>(userdata);
  if (filelist && *filelist) {
    auto& gameConfig = game->GetRegistry()->Get<GameConfig>();
    gameConfig.GetEngineOptions().lastProjectPath = *filelist;
    gameConfig.SaveGlobalPreferences();
    Logger::Info("Project folder selected: " + std::string(*filelist));
  }
}

void SDLCALL OnSceneFileSelected(void* userdata, const char* const* filelist, int /*filter*/) {
  auto* game = static_cast<Game*>(userdata);
  if (filelist && *filelist) {
    auto& gameConfig = game->GetRegistry()->Get<GameConfig>();
    auto& assetManager = game->GetRegistry()->Get<AssetManager>();
    std::filesystem::path selectedPath(*filelist);
    std::filesystem::path basePath(assetManager.GetBasePath());

    // Try to make it relative to the project root
    try {
      if (selectedPath.string().find(basePath.string()) == 0) {
        selectedPath = std::filesystem::relative(selectedPath, basePath);
      }
    } catch (const std::exception& e) {
      Logger::Warn("Failed to make scene path relative: " + std::string(e.what()));
    }

    gameConfig.GetEngineOptions().currentScenePath = selectedPath.string();
    Logger::Info("Scene file selected: " + selectedPath.string());
  }
}
#endif
}  // namespace

void RenderDebugGUISystem::Render(Game* game, SDL_Renderer* renderer, [[maybe_unused]] SDL_Texture* gameTexture, const float deltaTime) {
  auto* registry = game->GetRegistry();
  auto& gameConfig = registry->Get<GameConfig>();
  auto& engineOptions = gameConfig.GetEngineOptions();
  const bool projectLoaded = gameConfig.HasLoadedConfig();
  const bool editorSession = gameConfig.IsEditorMode() || !projectLoaded;

  [[maybe_unused]] const bool showEditorUI = editorSession;
  const bool showGameOverlays = engineOptions.showDebugGUI;

#ifdef OCTARINE_WITH_EDITOR
  if (!showEditorUI && !showGameOverlays && !engineOptions.showFpsCounter) {
    return;
  }
#else
  if (!showGameOverlays && !engineOptions.showFpsCounter) {
    return;
  }
#endif

  PROFILE_NAMED_SCOPE("RenderDebugGUISystem::Render");

  ImGui_ImplSDLRenderer3_NewFrame();
  ImGui_ImplSDL3_NewFrame();
  ImGui::NewFrame();

#ifdef OCTARINE_WITH_EDITOR
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
        ImGui::MenuItem("Scene View", nullptr, &engineOptions.showSceneWindow);
        ImGui::MenuItem("Scene Management", nullptr, &engineOptions.showSceneManagement);
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

    if (engineOptions.showSceneWindow) SceneWindow(gameTexture, &engineOptions.showSceneWindow);

    // --- Editor Mode Windows ---
    if (engineOptions.showSceneManagement) SceneManagementWindow(game);
    if (engineOptions.showHierarchy) HierarchyWindow(registry);
    if (engineOptions.showAssetBrowser) AssetBrowserWindow(registry);
    if (engineOptions.showLuaConsole) LuaConsoleWindow(game->GetLua());
    if (engineOptions.showProfiler) PerformanceProfilerWindow();
    EngineOptionsWindow(engineOptions);
    EditorSettingsWindow(engineOptions);

    if (engineOptions.showImGuiDemoWindow) {
      ImGui::ShowDemoWindow();
    }
  }
#endif

  if (showGameOverlays) {
    if (projectLoaded) {
      auto query = registry->CreateQuery<ScriptComponent>();
      RenderDebugGUISystem system;
      query->ForEach(system);
    }
    EntityInfoWindow(registry);
    FPSWindow(deltaTime);
  } else if (engineOptions.showFpsCounter) {
    FPSWindow(deltaTime);
  }

#ifdef OCTARINE_WITH_EDITOR
  if (!projectLoaded || showProjectSelector) {
    ProjectSelectorWindow(game, &showProjectSelector);
  }
#endif

  ImGui::Render();
  ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
}

#ifdef OCTARINE_WITH_EDITOR
void RenderDebugGUISystem::SceneManagementWindow(Game* game) {
  auto* registry = game->GetRegistry();
  auto& options = registry->Get<GameConfig>().GetEngineOptions();

  ImGui::Begin("Scene Management", &options.showSceneManagement);

  if (ImGui::Button("Stop Scene")) {
    game->StopScene();
  }
  ImGui::SameLine();
  if (ImGui::Button("Reload Scene")) {
    game->ReloadScene();
  }

  ImGui::Separator();
  ImGui::Text("Scene Script Path:");
  static char scenePath[256] = "";
  strncpy(scenePath, options.currentScenePath.c_str(), sizeof(scenePath));
  ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - 120);
  ImGui::InputText("##scenepath", scenePath, sizeof(scenePath));
  ImGui::SameLine();
  if (ImGui::Button("Browse...")) {
    SDL_ShowOpenFileDialog(OnSceneFileSelected, game, game->GetWindow(), nullptr, 0, nullptr, false);
  }

  if (ImGui::Button("Load Scene")) {
    game->LoadScene(scenePath);
  }

  ImGui::End();
}

void RenderDebugGUISystem::EngineOptionsWindow(EngineOptions& options) {
  // Always visible if editor is on
  ImGui::Begin("Engine Options");
  ImGui::Checkbox("Pause Game", &options.isPaused);
  if (options.isPaused) {
    ImGui::SameLine();
    if (ImGui::Button("Step Frame")) {
      options.stepFrame = true;
    }
  }
  ImGui::SliderFloat("Time Scale", &options.timeScale, 0.0f, 5.0f);
  ImGui::Separator();
  ImGui::Checkbox("Draw Colliders", &options.drawColliders);
  ImGui::Checkbox("Audio Enabled", &options.audioEnabled);
  ImGui::SliderFloat("Master Volume", &options.masterVolume, 0.0f, 1.0f);
  ImGui::End();
}

void RenderDebugGUISystem::EditorSettingsWindow(EngineOptions& options) {
  // Always visible if editor is on
  ImGui::Begin("Editor Settings");
  ImGui::SliderFloat("Font Size", &options.editorFontSize, 8.0f, 48.0f);
  if (ImGui::IsItemDeactivatedAfterEdit()) {
    Logger::Info("Editor font size changed, restart required for full effect.");
  }

  const char* styles[] = {"Dark", "Light", "Classic"};
  if (ImGui::Combo("Editor Style", &options.editorStyleIndex, styles, IM_ARRAYSIZE(styles))) {
    ApplyEditorStyle(options.editorStyleIndex);
  }
  ImGui::End();
}
#endif

void RenderDebugGUISystem::FPSWindow(const float deltaTime) {
  static float fpsHistory[120] = {};
  static int fpsOffset = 0;
  const float fps = (deltaTime > 0.0f) ? 1.0f / deltaTime : 0.0f;
  fpsHistory[fpsOffset] = fps;
  fpsOffset = (fpsOffset + 1) % 120;

  float avgFps = 0;
  for (float f : fpsHistory) avgFps += f;
  avgFps /= 120.0f;

  ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
  ImGui::Begin("Performance", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);
  ImGui::Text("FPS: %.1f (avg: %.1f)", static_cast<double>(fps), static_cast<double>(avgFps));
  ImGui::PlotLines("##fps", fpsHistory, 120, fpsOffset, nullptr, 0.0f, 120.0f, ImVec2(150, 40));
  ImGui::End();
}

void RenderDebugGUISystem::EntityInfoWindow(const Registry* registry) {
  ImGui::SetNextWindowPos(ImVec2(10, 80), ImGuiCond_FirstUseEver);
  ImGui::Begin("Entity Info", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
  ImGui::Text("User Entities: %zu", registry->GetUserEntityCount());
  ImGui::End();
}

#ifdef OCTARINE_WITH_EDITOR
void RenderDebugGUISystem::PerformanceProfilerWindow() {
#ifdef OCTARINE_PROFILING
  ImGui::Begin("Performance Profiler");
  if (ImGui::Button("Clear Max Values")) {
    PerfUtils::ProfilingAccumulator::Clear();
  }

  if (ImGui::BeginTable("ProfilingTable", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
    ImGui::TableSetupColumn("Metric");
    ImGui::TableSetupColumn("Last (ms)");
    ImGui::TableSetupColumn("Max (ms)");
    ImGui::TableHeadersRow();

    const auto& lastTimers = PerfUtils::ProfilingAccumulator::GetLastTimers();
    const auto& maxTimers = PerfUtils::ProfilingAccumulator::GetMaxTimers();

    for (const auto& [name, value] : lastTimers) {
      ImGui::TableNextRow();
      ImGui::TableSetColumnIndex(0);
      ImGui::Text("%s", name.c_str());
      ImGui::TableSetColumnIndex(1);
      ImGui::Text("%.3f", value);
      ImGui::TableSetColumnIndex(2);
      ImGui::Text("%.3f", maxTimers.at(name));
    }
    ImGui::EndTable();
  }

  ImGui::Separator();
  ImGui::Text("System Accumulators (Collision, etc):");
  if (ImGui::BeginTable("AccumTable", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
    ImGui::TableSetupColumn("Accumulator");
    ImGui::TableSetupColumn("Total (ms)");
    ImGui::TableHeadersRow();

    const auto& accumulators = PerfUtils::ProfilingAccumulator::GetLastAccumulators();
    for (const auto& [name, value] : accumulators) {
      ImGui::TableNextRow();
      ImGui::TableSetColumnIndex(0);
      ImGui::Text("%s", name.c_str());
      ImGui::TableSetColumnIndex(1);
      ImGui::Text("%.3f", value);
    }
    ImGui::EndTable();
  }
  ImGui::End();
#else
  ImGui::Begin("Performance Profiler");
  ImGui::Text("Profiling is disabled.");
  ImGui::Text("Rebuild with OCTARINE_ENABLE_PROFILING=ON to see data.");
  ImGui::End();
#endif
}

void RenderDebugGUISystem::HierarchyWindow(Registry* registry) {
  static auto selectedEntity = Entity();

  ImGui::Begin("Hierarchy");
  const auto& entities = registry->GetUserEntities();
  for (const auto& entity : entities) {
    std::string label = "Entity " + std::to_string(entity.GetId());
    if (registry->HasComponent<TransformComponent>(entity)) {
      label += " (Transform)";
    }

    if (ImGui::Selectable(label.c_str(), selectedEntity == entity)) {
      selectedEntity = entity;
    }
  }
  ImGui::End();

  ImGui::Begin("Inspector");
  if (registry->IsAlive(selectedEntity)) {
    ImGui::Text("ID: %u", selectedEntity.GetId());
    ImGui::Separator();

    if (registry->HasComponent<TransformComponent>(selectedEntity)) {
      if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
        auto& t = registry->GetComponent<TransformComponent>(selectedEntity);
        ImGui::DragFloat2("Position", &t.position.x, 1.0f);
        ImGui::DragFloat2("Scale", &t.scale.x, 0.1f);
        float rotation = static_cast<float>(t.rotation);
        if (ImGui::DragFloat("Rotation", &rotation, 1.0f)) {
          t.rotation = rotation;
        }
      }
    }
    // Add more component editors here...
  } else {
    ImGui::Text("Select an entity in the Hierarchy.");
  }
  ImGui::End();
}

void RenderDebugGUISystem::ProjectSelectorWindow(Game* game, bool* p_open) {
  ImGui::OpenPopup("Project Selector");
  if (ImGui::BeginPopupModal("Project Selector", p_open, ImGuiWindowFlags_AlwaysAutoResize)) {
    auto& gameConfig = game->GetRegistry()->Get<GameConfig>();
    static char projectPath[256] = "";
    strncpy(projectPath, gameConfig.GetEngineOptions().lastProjectPath.c_str(), sizeof(projectPath));

    ImGui::Text("Select a project folder to load:");
    ImGui::SetNextItemWidth(300);
    ImGui::InputText("##path", projectPath, sizeof(projectPath));
    ImGui::SameLine();
    if (ImGui::Button("Browse...")) {
      SDL_ShowOpenFileDialog(OnProjectFolderSelected, game, game->GetWindow(), nullptr, 0, nullptr, false);
    }

    ImGui::Separator();
    if (ImGui::Button("Load Project", ImVec2(120, 0))) {
      gameConfig.LoadConfigFromFile(projectPath);
      ImGui::CloseCurrentPopup();
      *p_open = false;
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel", ImVec2(120, 0))) {
      ImGui::CloseCurrentPopup();
      *p_open = false;
    }
    ImGui::EndPopup();
  }
}

void RenderDebugGUISystem::SceneWindow(SDL_Texture* gameTexture, bool* p_open) {
  ImGui::Begin("Scene", p_open);
  const ImVec2 viewportSize = ImGui::GetContentRegionAvail();
  if (gameTexture) {
    ImGui::Image(reinterpret_cast<void*>(gameTexture), viewportSize);
  }
  ImGui::End();
}

void RenderDebugGUISystem::AssetBrowserWindow(Registry* registry) {
  ImGui::Begin("Asset Browser");
  auto& am = registry->Get<AssetManager>();
  if (ImGui::CollapsingHeader("Textures", ImGuiTreeNodeFlags_DefaultOpen)) {
    for (const auto& [id, texture] : am.GetTextures()) {
      ImGui::Text("%s", id.c_str());
    }
  }
  ImGui::End();
}

void RenderDebugGUISystem::LuaConsoleWindow(sol::state& lua) {
  static char inputBuffer[256] = "";
  static bool scrollToBottom = false;
  static bool reclaimFocus = false;

  ImGui::Begin("Lua Console");

  const float footerHeightToReserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
  ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footerHeightToReserve), false, ImGuiWindowFlags_HorizontalScrollbar);

  Logger::ForEachHistory([](const std::string& line) {
    ImGui::TextUnformatted(line.c_str());
  });

  if (scrollToBottom || ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
    ImGui::SetScrollHereY(1.0f);
  }
  scrollToBottom = false;
  ImGui::EndChild();

  ImGui::Separator();

  if (ImGui::InputText("Input", inputBuffer, sizeof(inputBuffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
    std::string input(inputBuffer);
    if (!input.empty()) {
      Logger::LogLua("> " + input);
      auto result = lua.safe_script(input, sol::script_pass_on_error);
      if (!result.valid()) {
        sol::error err = result;
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
#endif
#endif
