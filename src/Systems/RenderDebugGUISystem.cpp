#include "RenderDebugGUISystem.h"

#ifdef OCTARINE_WITH_IMGUI
#include <filesystem>

#include "../AssetManager/AssetManager.h"
#include "../Components/AnimationComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/CameraFollowComponent.h"
#include "../Components/EntityMaskComponent.h"
#include "../Components/GlobalTransformComponent.h"
#include "../Components/HealthComponent.h"
#include "../Components/NameComponent.h"
#include "../Components/ViewportInfo.h"
#include "../Components/PositionComponent.h"
#include "../Components/ProjectileComponent.h"
#include "../Components/ProjectileEmitterComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/RotationComponent.h"
#include "../Components/ScaleComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/SquarePrimitiveComponent.h"
#include "../Components/TextLabelComponent.h"
#include "../Components/UIButtonComponent.h"
#include "../Game/Game.h"
#include "../Game/GameConfig.h"
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"
#include "imgui_internal.h"  // BeginViewportSideBar (playback toolbar pinned under the menu bar)

#ifdef OCTARINE_WITH_EDITOR
#include "../Editor/EditorLayoutPresets.h"
#include "../Editor/EditorPersistence.h"
#include "../Editor/Inspectors/ComponentInspectorRegistry.h"
#include "../Editor/Inspectors/InspectorWidgets.h"

namespace
{
    void SDLCALL OnProjectFolderSelected(void* userdata, const char* const* filelist, int /*filter*/)
    {
        auto* game = static_cast<Game*>(userdata);
        if (filelist && *filelist)
        {
            auto& editorPersistence = game->GetRegistry()->Get<EditorPersistence>();
            editorPersistence.lastProjectPath = *filelist;
            editorPersistence.SaveGlobal();
            Logger::Info("Project folder selected: " + std::string(*filelist));
        }
    }

    void SDLCALL OnSceneFileSelected(void* userdata, const char* const* filelist, int /*filter*/)
    {
        auto* game = static_cast<Game*>(userdata);
        if (filelist && *filelist)
        {
            auto& editorPersistence = game->GetRegistry()->Get<EditorPersistence>();
            auto& assetManager = game->GetRegistry()->Get<AssetManager>();
            const std::filesystem::path selectedPath(*filelist);
            const std::filesystem::path basePath(assetManager.GetBasePath());

            // Prefer a project-relative path (e.g. "scripts/level1.lua"), but only when the file
            // actually lives inside the project — a relative result starting with ".." escapes it,
            // so keep the absolute path in that case.
            std::error_code ec;
            const std::filesystem::path relativePath = std::filesystem::relative(selectedPath, basePath, ec);
            const bool insideProject = !ec && !relativePath.empty() && relativePath.begin()->string() != "..";
            const std::string scenePath = insideProject ? relativePath.generic_string() : selectedPath.string();

            editorPersistence.currentScenePath = scenePath;
            Logger::Info("Scene file selected: " + scenePath);
        }
    }
} // namespace
#endif

void RenderDebugGUISystem::Render(Game * game, SDL_Renderer * renderer,
#ifdef OCTARINE_WITH_EDITOR
SDL_Texture *gameTexture,
#else
SDL_Texture* /*gameTexture*/,
#endif
const float deltaTime) {
  auto* registry = game->GetRegistry();
  auto& gameConfig = registry->Get<GameConfig>();
  auto& engineOptions = gameConfig.GetEngineOptions();
  const bool projectLoaded = gameConfig.HasLoadedConfig();
#ifdef OCTARINE_WITH_EDITOR
auto& editorPersistence = registry->Get<EditorPersistence>();
const bool editorSession = gameConfig.IsEditorMode() || !projectLoaded;
const bool showEditorUI = editorSession;
#endif
const bool showGameOverlays = engineOptions.showDebugGUI;

#ifdef OCTARINE_WITH_EDITOR
if (!showEditorUI&&!showGameOverlays&&!engineOptions.showFpsCounter) {
    return;
  }
#else
if (!showGameOverlays&&!engineOptions.showFpsCounter) {
    return;
  }
#endif

PROFILE_NAMED_SCOPE ("RenderDebugGUISystem::Render");

#ifdef OCTARINE_WITH_EDITOR
if (!editorPersistence.pendingLayoutLoad.empty()) {
    if (editorPersistence.pendingLayoutLoad == "__default__") {
      octarine::editor::layouts::ApplyDefaultPreset(editorPersistence);
    } else {
      octarine::editor::layouts::LoadPreset(editorPersistence.pendingLayoutLoad, editorPersistence);
    }
    editorPersistence.pendingLayoutLoad.clear();
  }
#endif

ImGui_ImplSDLRenderer3_NewFrame();
ImGui_ImplSDL3_NewFrame();
ImGui::NewFrame();

#ifdef OCTARINE_WITH_EDITOR
static bool showProjectSelector = false;
static bool openSaveLayoutModal = false;

  if (showEditorUI)
{
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0));
    ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());
    ImGui::PopStyleColor();

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Open Project...", "Ctrl+O"))
            {
                showProjectSelector = true;
            }
            if (ImGui::MenuItem("Save Preferences", "Ctrl+S"))
            {
                gameConfig.SaveUserPreferences();
                editorPersistence.audioMuted = !engineOptions.audioEnabled;
                editorPersistence.masterVolume = engineOptions.masterVolume;
                editorPersistence.SaveGlobal();
                if (projectLoaded) editorPersistence.SaveProject(gameConfig.GetAssetPath());
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Quit", "Alt+F4"))
            {
                Game::Quit();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Layout"))
        {
            const auto presets = octarine::editor::layouts::ListPresets();
            if (ImGui::BeginMenu("Apply", !presets.empty()))
            {
                for (const auto& name : presets)
                {
                    if (ImGui::MenuItem(name.c_str()))
                    {
                        editorPersistence.pendingLayoutLoad = name;
                    }
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Delete", !presets.empty()))
            {
                for (const auto& name : presets)
                {
                    if (ImGui::MenuItem(name.c_str()))
                    {
                        octarine::editor::layouts::DeletePreset(name);
                    }
                }
                ImGui::EndMenu();
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Save Current As..."))
            {
                openSaveLayoutModal = true;
            }
            if (ImGui::MenuItem("Reset to Default"))
            {
                editorPersistence.pendingLayoutLoad = "__default__";
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Windows"))
        {
            ImGui::MenuItem("Scene View", nullptr, &editorPersistence.showSceneWindow);
            ImGui::MenuItem("Scene Management", nullptr, &editorPersistence.showSceneManagement);
            ImGui::MenuItem("Hierarchy", nullptr, &editorPersistence.showHierarchy);
            ImGui::MenuItem("Asset Browser", nullptr, &editorPersistence.showAssetBrowser);
            ImGui::MenuItem("Lua Console", nullptr, &editorPersistence.showLuaConsole);
            ImGui::MenuItem("Performance Profiler", nullptr, &editorPersistence.showProfiler);
            ImGui::MenuItem("Engine Options", nullptr, &editorPersistence.showEngineOptions);
            ImGui::MenuItem("Editor Settings", nullptr, &editorPersistence.showEditorSettings);
            ImGui::MenuItem("Game Debug Overlays", "Grave", &engineOptions.showDebugGUI);
            ImGui::Separator();
            ImGui::MenuItem("FPS Counter", nullptr, &engineOptions.showFpsCounter);
            ImGui::MenuItem("ImGui Demo Window", nullptr, &engineOptions.showImGuiDemoWindow);
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    // Playback toolbar: a fixed full-width bar pinned directly under the main menu bar.
    // Submitting it as an Up side-bar after the menu bar stacks it beneath the menu.
    {
        ImGuiViewport* vp = ImGui::GetMainViewport();
        if (ImGui::BeginViewportSideBar("##PlaybackToolbar", vp, ImGuiDir_Up, ImGui::GetFrameHeight(),
                                        ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar))
        {
            if (ImGui::BeginMenuBar())
            {
                const bool hasScene = !editorPersistence.currentScenePath.empty();

                // Play — resume execution. If a configured scene was stopped, (re)start it first.
                // Disabled while already running so the button reads as the current state.
                ImGui::BeginDisabled(!engineOptions.isPaused);
                if (ImGui::Button("Play"))
                {
                    if (!game->IsSceneRunning() && hasScene)
                    {
                        game->ReloadScene();
                    }
                    engineOptions.isPaused = false;
                }
                ImGui::EndDisabled();
                ImGui::SameLine();
                ImGui::BeginDisabled(engineOptions.isPaused);
                if (ImGui::Button("Pause"))
                {
                    engineOptions.isPaused = true;
                }
                ImGui::EndDisabled();
                ImGui::SameLine();
                if (ImGui::Button("Step")) // freeze, then advance exactly one frame
                {
                    engineOptions.isPaused = true;
                    engineOptions.stepFrame = true;
                }
                ImGui::SameLine();
                if (ImGui::Button("Stop"))
                {
                    game->StopScene();
                    engineOptions.isPaused = true; // back to the default ready-but-paused state
                }

                ImGui::SameLine();
                if (ImGui::Button(engineOptions.audioEnabled ? "Mute" : "Unmute"))
                {
                    engineOptions.audioEnabled = !engineOptions.audioEnabled;
                    editorPersistence.audioMuted = !engineOptions.audioEnabled;
                    editorPersistence.masterVolume = engineOptions.masterVolume;
                    editorPersistence.SaveGlobal(); // persist immediately
                }
                ImGui::EndMenuBar();
            }
        }
        ImGui::End();
    }

    if (openSaveLayoutModal)
    {
        ImGui::OpenPopup("Save Layout Preset");
        openSaveLayoutModal = false;
    }
    if (ImGui::BeginPopupModal("Save Layout Preset", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        static char nameBuf[64] = "";
        ImGui::Text("Preset name:");
        ImGui::SetNextItemWidth(260);
        const bool enter = ImGui::InputText("##layoutname", nameBuf, sizeof(nameBuf),
                                            ImGuiInputTextFlags_EnterReturnsTrue);
        const bool hasName = nameBuf[0] != '\0';
        ImGui::BeginDisabled(!hasName);
        if (ImGui::Button("Save") || (enter && hasName))
        {
            octarine::editor::layouts::SavePreset(nameBuf, editorPersistence);
            nameBuf[0] = '\0';
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndDisabled();
        ImGui::SameLine();
        if (ImGui::Button("Cancel"))
        {
            nameBuf[0] = '\0';
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    if (editorPersistence.showSceneWindow) SceneWindow(game, gameTexture, &editorPersistence.showSceneWindow);

    if (editorPersistence.showSceneManagement) SceneManagementWindow(game);
    if (editorPersistence.showHierarchy) HierarchyWindow(registry);
    if (editorPersistence.showAssetBrowser) AssetBrowserWindow(registry);
    if (editorPersistence.showLuaConsole) LuaConsoleWindow(game->GetLua());
    if (editorPersistence.showProfiler) PerformanceProfilerWindow();
    if (editorPersistence.showEngineOptions)
        EngineOptionsWindow(engineOptions, &editorPersistence.showEngineOptions);
    if (editorPersistence.showEditorSettings)
        EditorSettingsWindow(editorPersistence, &editorPersistence.showEditorSettings);

    if (engineOptions.showImGuiDemoWindow)
    {
        ImGui::ShowDemoWindow();
    }
}
#endif

if (showGameOverlays)
{
    if (projectLoaded)
    {
        auto query = registry->CreateQuery<ScriptComponent>();
        RenderDebugGUISystem system;
        query->ForEach(system);
    }
    EntityInfoWindow(registry);
    FPSWindow(deltaTime);
}else if (engineOptions.showFpsCounter) {
    FPSWindow(deltaTime);
  }

#ifdef OCTARINE_WITH_EDITOR
if (!projectLoaded|| showProjectSelector) {
    ProjectSelectorWindow(game, &showProjectSelector);
  }
#endif

ImGui::Render();
ImGui_ImplSDLRenderer3_RenderDrawData (ImGui::GetDrawData(), renderer);

#ifdef OCTARINE_WITH_EDITOR
// Honor any deferred font-rebuild requests now that the ImGui frame has ended.
// Atlas rebuilds inside an active frame would corrupt draw state.
if (editorPersistence.fontRebuildPending) {
    RebuildEditorFont(editorPersistence.editorFontSize);
    ApplyEditorStyle(editorPersistence.editorStyleIndex, editorPersistence.editorFontSize);
    editorPersistence.fontRebuildPending = false;
  }
#endif
}

#ifdef OCTARINE_WITH_EDITOR
void RenderDebugGUISystem::SceneManagementWindow(Game* game)
{
    auto* registry = game->GetRegistry();
    auto& editorPersistence = registry->Get<EditorPersistence>();

    ImGui::Begin("Scene Management", &editorPersistence.showSceneManagement);

    static std::string scenePath;
    static std::string lastKnownScenePath;
    if (lastKnownScenePath != editorPersistence.currentScenePath)
    {
        scenePath = editorPersistence.currentScenePath;
        lastKnownScenePath = editorPersistence.currentScenePath;
    }

    ImGui::Text("Scene Script Path:");
    // Reserve exactly the width of the trailing "..." + "Load" buttons so the input field doesn't
    // push them off-screen at large editor font sizes.
    const ImGuiStyle& style = ImGui::GetStyle();
    const float sceneButtonsWidth = ImGui::CalcTextSize("...").x + ImGui::CalcTextSize("Load").x +
        style.FramePadding.x * 4.0F + style.ItemSpacing.x * 2.0F;
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - sceneButtonsWidth);
    octarine::editor::inspectors::InputTextString("##scenepath", scenePath);
    ImGui::SameLine();
    if (ImGui::Button("..."))
    {
        const SDL_DialogFileFilter filters[] = {{"Lua Scripts", "lua"}, {"All Files", "*"}};
        SDL_ShowOpenFileDialog(OnSceneFileSelected, game, game->GetWindow(), filters, 2, nullptr, false);
    }
    ImGui::SameLine();
    if (ImGui::Button("Load"))
    {
        game->LoadScene(scenePath);
    }

    ImGui::Separator();

    if (!editorPersistence.currentScenePath.empty())
    {
        ImGui::Text("Current Scene: %s", editorPersistence.currentScenePath.c_str());
        if (ImGui::Button("Reload"))
        {
            game->ReloadScene();
        }
        ImGui::SameLine();
        if (ImGui::Button("Stop"))
        {
            game->StopScene();
        }
    }
    else
    {
        ImGui::Text("No scene currently loaded.");
    }

    ImGui::Separator();
    ImGui::TextDisabled("Enter path relative to project root (e.g. scripts/level1.lua)");

    ImGui::End();
}

void RenderDebugGUISystem::EngineOptionsWindow(EngineOptions& options, bool* p_open)
{
    // Runtime engine options only. Window show/hide lives in the Windows menu (single source of truth).
    ImGui::Begin("Engine Options", p_open);
    ImGui::Checkbox("Show ImGui Demo Window", &options.showImGuiDemoWindow);
    ImGui::Checkbox("Show FPS Counter", &options.showFpsCounter);
    ImGui::Checkbox("Show Entity Info", &options.showEntityInfo);
    ImGui::Checkbox("Draw Colliders", &options.drawColliders);
    ImGui::Checkbox("Log Input Events", &options.logInputEvents);
    ImGui::Checkbox("Audio Enabled", &options.audioEnabled);
    ImGui::SliderFloat("Master Volume", &options.masterVolume, 0.0F, 1.0F);
    ImGui::Separator();
    ImGui::Checkbox("Pause Execution", &options.isPaused);
    ImGui::SameLine();
    if (ImGui::Button("Step Frame"))
    {
        options.stepFrame = true;
    }
    ImGui::SliderFloat("Time Scale", &options.timeScale, 0.0F, 5.0F);
    ImGui::End();
}

void RenderDebugGUISystem::EditorSettingsWindow(EditorPersistence& editorPersistence, bool* p_open)
{
    ImGui::Begin("Editor Settings", p_open);

    ImGui::SeparatorText("Appearance");

    float fontSize =
        (editorPersistence.editorFontSize > 0.0F) ? editorPersistence.editorFontSize : kBaselineFontSize;
    if (ImGui::SliderFloat("Font Size", &fontSize, 10.0F, 40.0F, "%.0f px"))
    {
        editorPersistence.editorFontSize = fontSize;
    }
    // Defer the actual atlas rebuild until the user releases the slider — rebuilding
    // every frame while dragging is visibly laggy.
    if (ImGui::IsItemDeactivatedAfterEdit())
    {
        editorPersistence.fontRebuildPending = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("Reset"))
    {
        editorPersistence.editorFontSize = kBaselineFontSize;
        editorPersistence.fontRebuildPending = true;
    }

    const char* styles[] = {"Dark", "Light", "Classic"};
    if (ImGui::Combo("Theme", &editorPersistence.editorStyleIndex, styles, IM_ARRAYSIZE(styles)))
    {
        ApplyEditorStyle(editorPersistence.editorStyleIndex, editorPersistence.editorFontSize);
    }

    ImGui::End();
}
#endif

void RenderDebugGUISystem::FPSWindow(const float deltaTime)
{
    static float fpsHistory[120] = {};
    static int fpsOffset = 0;
    const float fps = (deltaTime > 0.0f) ? 1.0f / deltaTime : 0.0f;
    fpsHistory[fpsOffset] = fps;
    fpsOffset = (fpsOffset + 1) % 120;

    ImGui::Begin("FPS");
    char overlay[32];
    snprintf(overlay, sizeof(overlay), "%.1f FPS", static_cast<double>(fps));
    ImGui::PlotLines("##fps", fpsHistory, 120, fpsOffset, overlay, 0.0f, 120.0f, ImVec2(0, 50));
    ImGui::End();
}

void RenderDebugGUISystem::EntityInfoWindow(const Registry* registry)
{
    const auto count = registry->GetUserEntityCount();
    ImGui::Begin("Entity Info");
    ImGui::Text("Entity Count: %llu", static_cast<unsigned long long>(count));
    ImGui::End();
}

#ifdef OCTARINE_WITH_EDITOR
void RenderDebugGUISystem::PerformanceProfilerWindow()
{
    ImGui::Begin("Performance Profiler");
    const auto times = PerfUtils::ProfilingAccumulator::GetAccumulatedTimes();
    if (ImGui::BeginTable("profiler_table", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
    {
        ImGui::TableSetupColumn("System/Scope");
        ImGui::TableSetupColumn("Time (ms)");
        ImGui::TableHeadersRow();

        for (const auto& [name, duration_us] : times)
        {
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
    if (!counters.empty() && ImGui::BeginTable("counters_table", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
    {
        ImGui::TableSetupColumn("Counter");
        ImGui::TableSetupColumn("Value");
        ImGui::TableHeadersRow();

        for (const auto& [name, value] : counters)
        {
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

void RenderDebugGUISystem::HierarchyWindow(Registry* registry)
{
    ImGui::Begin("Hierarchy / Entity Inspector");
    const auto entities = registry->GetUserEntities();
    ImGui::Text("Total User Entities: %zu", entities.size());
    if (ImGui::Button("Create Entity"))
    {
        registry->CreateEntity();
    }

    static Entity selectedEntity{UINT64_MAX};
    static ImGuiTextFilter entityFilter;
    entityFilter.Draw("##filter", 140);

    ImGui::BeginChild("EntityList", ImVec2(150, 0), true);
    for (const auto& entity : entities)
    {
        std::string label;
        if (registry->HasComponent<NameComponent>(entity))
        {
            label = registry->GetComponent<NameComponent>(entity).name;
        }
        if (label.empty()) label = "Entity " + std::to_string(entity.id);
        if (!entityFilter.PassFilter(label.c_str())) continue;
        if (ImGui::Selectable(label.c_str(), selectedEntity == entity))
        {
            selectedEntity = entity;
        }
    }
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("EntityDetails", ImVec2(0, 0), true);
    if (registry->IsAlive(selectedEntity))
    {
        ImGui::Text("Selected Entity ID: %u", selectedEntity.GetId());
        ImGui::SameLine();
        if (ImGui::Button("Destroy Entity"))
        {
            registry->BlamEntity(selectedEntity);
            selectedEntity = Entity{UINT64_MAX};
        }
        if (registry->IsAlive(selectedEntity))
        {
            std::string name;
            if (registry->HasComponent<NameComponent>(selectedEntity))
            {
                name = registry->GetComponent<NameComponent>(selectedEntity).name;
            }
            if (octarine::editor::inspectors::InputTextString("Name", name))
            {
                registry->AddComponent(selectedEntity, NameComponent(name));
            }
        }
        ImGui::Separator();

        // Per-component inspector blocks are driven by ComponentInspectorRegistry — each entry
        // renders its own collapsing header when the entity has that component.
        for (const auto& inspector : ComponentInspectorRegistry::all())
        {
            inspector.draw(registry, selectedEntity);
        }

        ImGui::Separator();
        if (ImGui::BeginCombo("Add Component", "Select..."))
        {
            for (const auto& inspector : ComponentInspectorRegistry::all())
            {
                if (inspector.addDefault && !inspector.has(registry, selectedEntity) &&
                    ImGui::Selectable(inspector.displayName.c_str()))
                {
                    inspector.addDefault(registry, selectedEntity);
                }
            }
            ImGui::EndCombo();
        }
    }
    else
    {
        ImGui::Text("No entity selected or entity destroyed.");
    }
    ImGui::EndChild();
    ImGui::End();
}

void RenderDebugGUISystem::ProjectSelectorWindow(Game* game, bool* p_open)
{
    ImGui::SetNextWindowSize(ImVec2(400, 240), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin("Project Selector", p_open, ImGuiWindowFlags_NoDocking))
    {
        ImGui::End();
        return;
    }
    auto& editorPersistence = game->GetRegistry()->Get<EditorPersistence>();
    static std::string projectPath;
    static std::string lastKnownProjectPath;
    if (lastKnownProjectPath != editorPersistence.lastProjectPath)
    {
        projectPath = editorPersistence.lastProjectPath;
        lastKnownProjectPath = editorPersistence.lastProjectPath;
    }

    ImGui::Text("Enter Project Path:");
    // Reserve the trailing "..." button width so the input scales with the editor font size.
    const ImGuiStyle& style = ImGui::GetStyle();
    const float browseButtonWidth = ImGui::CalcTextSize("...").x + style.FramePadding.x * 2.0F + style.ItemSpacing.x;
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - browseButtonWidth);
    octarine::editor::inspectors::InputTextString("##path", projectPath);
    ImGui::SameLine();
    if (ImGui::Button("..."))
    {
        SDL_ShowOpenFolderDialog(OnProjectFolderSelected, game, game->GetWindow(), nullptr, false);
    }

    if (ImGui::Button("Open Project", ImVec2(ImGui::GetContentRegionAvail().x, 0)))
    {
        editorPersistence.lastProjectPath = projectPath;
        editorPersistence.SaveGlobal();
        *p_open = false;
        ImGui::OpenPopup("Restart Required");
    }
    if (ImGui::BeginPopupModal("Restart Required", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Project path saved. Please restart the engine to load the new project.");
        if (ImGui::Button("OK", ImVec2(120, 0))) ImGui::CloseCurrentPopup();
        ImGui::EndPopup();
    }
    if (!editorPersistence.lastProjectPath.empty())
    {
        ImGui::Separator();
        ImGui::Text("Last Project: %s", editorPersistence.lastProjectPath.c_str());
        if (ImGui::Button("Load Last Project"))
            projectPath = editorPersistence.lastProjectPath;
    }
    ImGui::End();
}

void RenderDebugGUISystem::SceneWindow(Game* game, SDL_Texture* gameTexture, bool* p_open)
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    if (ImGui::Begin("Scene View", p_open))
    {
        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        if (gameTexture)
        {
            float texW = 0, texH = 0;
            SDL_GetTextureSize(gameTexture, &texW, &texH);
            if (texW > 0 && texH > 0)
            {
                float aspectRatio = texW / texH;
                float targetW = viewportPanelSize.x;
                float targetH = targetW / aspectRatio;
                if (targetH > viewportPanelSize.y)
                {
                    targetH = viewportPanelSize.y;
                    targetW = targetH * aspectRatio;
                }
                ImVec2 imageSize(targetW, targetH);

                // Center the image within the available content area.
                float offsetX = (viewportPanelSize.x - imageSize.x) * 0.5f;
                float offsetY = (viewportPanelSize.y - imageSize.y) * 0.5f;
                ImVec2 contentStartPos = ImGui::GetCursorPos();
                ImGui::SetCursorPos(ImVec2(contentStartPos.x + offsetX, contentStartPos.y + offsetY));

                // Record the image's screen rect (relative to the main viewport origin) so input
                // systems can map window-space cursor coordinates into the shrunken game viewport.
                ImVec2 imageScreenPos = ImGui::GetCursorScreenPos();
                ImVec2 windowPos = ImGui::GetMainViewport()->Pos;
                auto& viewportInfo = game->GetRegistry()->Get<ViewportInfo>();
                viewportInfo.x = imageScreenPos.x - windowPos.x;
                viewportInfo.y = imageScreenPos.y - windowPos.y;
                viewportInfo.width = imageSize.x;
                viewportInfo.height = imageSize.y;

                ImGui::Image(reinterpret_cast<ImTextureID>(gameTexture), imageSize);
                viewportInfo.isHovered = ImGui::IsItemHovered();
                viewportInfo.isFocused = ImGui::IsWindowFocused();
            }
        }
    }
    ImGui::End();
    ImGui::PopStyleVar();
}

void RenderDebugGUISystem::AssetBrowserWindow(Registry* registry)
{
    ImGui::Begin("Asset Browser");
    auto& assetManager = registry->Get<AssetManager>();
    if (ImGui::CollapsingHeader("Textures", ImGuiTreeNodeFlags_DefaultOpen))
    {
        const auto& textures = assetManager.GetTextures();
        ImGui::Text("Loaded: %zu", textures.size());
        for (const auto& [id, texture] : textures) ImGui::BulletText("%s", id.c_str());
    }
    if (ImGui::CollapsingHeader("Fonts", ImGuiTreeNodeFlags_DefaultOpen))
    {
        const auto& fonts = assetManager.GetFonts();
        ImGui::Text("Loaded: %zu", fonts.size());
        for (const auto& [id, font] : fonts) ImGui::BulletText("%s", id.c_str());
    }
    if (ImGui::CollapsingHeader("Audio Clips", ImGuiTreeNodeFlags_DefaultOpen))
    {
        const auto& audioClips = assetManager.GetAudioClips();
        ImGui::Text("Loaded: %zu", audioClips.size());
        for (const auto& [id, clip] : audioClips) ImGui::BulletText("%s", id.c_str());
    }
    ImGui::End();
}

void RenderDebugGUISystem::LuaConsoleWindow(sol::state& lua)
{
    static char inputBuffer[256] = "";
    static std::vector<std::string> commandHistory;
    static int commandHistoryIndex = -1;
    static bool scrollToBottom = false;
    static bool reclaimFocus = false;

    ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin("Lua Console"))
    {
        ImGui::End();
        return;
    }

    if (ImGui::Button("Clear"))
    {
        Logger::ClearHistory();
    }
    ImGui::SameLine();
    ImGui::TextDisabled("Type Lua commands below. Use Up/Down for history.");

    const float footerHeightToReserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
    ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footerHeightToReserve), false,
                      ImGuiWindowFlags_HorizontalScrollbar);

    Logger::ForEachHistory([](const std::string& line)
    {
        ImVec4 color = ImVec4(1.0F, 1.0F, 1.0F, 1.0F);
        if (line.find("[Error]") != std::string::npos)
        {
            color = ImVec4(1.0F, 0.4F, 0.4F, 1.0F);
        }
        else if (line.find("[Warn]") != std::string::npos)
        {
            color = ImVec4(1.0F, 1.0F, 0.4F, 1.0F);
        }
        else if (line.find("[Info]") != std::string::npos)
        {
            color = ImVec4(0.4F, 1.0F, 0.4F, 1.0F);
        }
        else if (line.find("> ") == 0)
        {
            color = ImVec4(0.4F, 0.8F, 1.0F, 1.0F);
        }
        ImGui::PushStyleColor(ImGuiCol_Text, color);
        ImGui::TextUnformatted(line.c_str());
        ImGui::PopStyleColor();
    });

    if (scrollToBottom)
    {
        ImGui::SetScrollHereY(1.0f);
    }
    scrollToBottom = false;
    ImGui::EndChild();

    ImGui::Separator();

    struct CallbackData
    {
        std::vector<std::string>* history;
        int* index;
    } cbData{&commandHistory, &commandHistoryIndex};

    auto wrappedCallback = [](ImGuiInputTextCallbackData* data) -> int
    {
        auto* cb = static_cast<CallbackData*>(data->UserData);
        if (data->EventFlag == ImGuiInputTextFlags_CallbackHistory)
        {
            if (data->EventKey == ImGuiKey_UpArrow)
            {
                if (*cb->index == -1)
                {
                    *cb->index = static_cast<int>(cb->history->size()) - 1;
                }
                else if (*cb->index > 0)
                {
                    (*cb->index)--;
                }
            }
            else if (data->EventKey == ImGuiKey_DownArrow)
            {
                if (*cb->index != -1)
                {
                    if (*cb->index < static_cast<int>(cb->history->size()) - 1)
                    {
                        (*cb->index)++;
                    }
                    else
                    {
                        *cb->index = -1;
                    }
                }
            }
            if (*cb->index != -1)
            {
                data->DeleteChars(0, data->BufTextLen);
                data->InsertChars(0, (*cb->history)[static_cast<size_t>(*cb->index)].c_str());
            }
            else
            {
                data->DeleteChars(0, data->BufTextLen);
            }
        }
        return 0;
    };

    ImGuiInputTextFlags inputTextFlags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackHistory;
    if (ImGui::InputText("##Input", inputBuffer, IM_ARRAYSIZE(inputBuffer), inputTextFlags, wrappedCallback, &cbData))
    {
        std::string command(inputBuffer);
        if (!command.empty())
        {
            Logger::LogLua("> " + command);
            commandHistory.push_back(command);
            commandHistoryIndex = -1;

            auto result = lua.safe_script(command, sol::script_pass_on_error);
            if (!result.valid())
            {
                const sol::error err = result;
                Logger::ErrorLua(std::string(err.what()));
            }

            inputBuffer[0] = '\0';
            scrollToBottom = true;
            reclaimFocus = true;
        }
    }

    // Only refocus the input right after submitting a command. Don't steal focus whenever the
    // window is focused — that prevented scrolling the log with the keyboard.
    if (reclaimFocus)
    {
        ImGui::SetKeyboardFocusHere(-1);
        reclaimFocus = false;
    }

    ImGui::End();
}
#endif
#endif
