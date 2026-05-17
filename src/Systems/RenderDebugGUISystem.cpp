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

#ifdef OCTARINE_WITH_EDITOR
#include "../Editor/EditorLayoutPresets.h"
#include "../Editor/EditorPersistence.h"

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
            std::filesystem::path selectedPath(*filelist);
            std::filesystem::path basePath(assetManager.GetBasePath());

            try
            {
                if (selectedPath.string().find(basePath.string()) == 0)
                {
                    selectedPath = std::filesystem::relative(selectedPath, basePath);
                }
            }
            catch (const std::exception& e)
            {
                Logger::Warn("Failed to make scene path relative: " + std::string(e.what()));
            }

            editorPersistence.currentScenePath = selectedPath.string();
            Logger::Info("Scene file selected: " + selectedPath.string());
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
            ImGui::MenuItem("Game Debug Overlays", "Grave", &engineOptions.showDebugGUI);
            ImGui::Separator();
            ImGui::MenuItem("FPS Counter", nullptr, &engineOptions.showFpsCounter);
            ImGui::MenuItem("ImGui Demo Window", nullptr, &engineOptions.showImGuiDemoWindow);
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
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

    if (editorPersistence.showSceneWindow) SceneWindow(gameTexture, &editorPersistence.showSceneWindow);

    if (editorPersistence.showSceneManagement) SceneManagementWindow(game);
    if (editorPersistence.showHierarchy) HierarchyWindow(registry);
    if (editorPersistence.showAssetBrowser) AssetBrowserWindow(registry);
    if (editorPersistence.showLuaConsole) LuaConsoleWindow(game->GetLua());
    if (editorPersistence.showProfiler) PerformanceProfilerWindow();
    EngineOptionsWindow(engineOptions, editorPersistence);
    EditorSettingsWindow(editorPersistence);

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

    static char scenePath[256] = "";
    static std::string lastKnownScenePath;
    if (lastKnownScenePath != editorPersistence.currentScenePath)
    {
        snprintf(scenePath, sizeof(scenePath), "%s", editorPersistence.currentScenePath.c_str());
        lastKnownScenePath = editorPersistence.currentScenePath;
    }

    ImGui::Text("Scene Script Path:");
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - 120);
    ImGui::InputText("##scenepath", scenePath, sizeof(scenePath));
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

void RenderDebugGUISystem::EngineOptionsWindow(EngineOptions& options, EditorPersistence& editorPersistence)
{
    ImGui::Begin("Engine Options");
    ImGui::Checkbox("Show ImGui Demo Window", &options.showImGuiDemoWindow);
    ImGui::Checkbox("Show FPS Counter", &options.showFpsCounter);
    ImGui::Checkbox("Show Entity Info", &options.showEntityInfo);
    ImGui::Checkbox("Show Profiler", &editorPersistence.showProfiler);
    ImGui::Checkbox("Show Hierarchy", &editorPersistence.showHierarchy);
    ImGui::Checkbox("Show Asset Browser", &editorPersistence.showAssetBrowser);
    ImGui::Checkbox("Show Lua Console", &editorPersistence.showLuaConsole);
    ImGui::Checkbox("Draw Colliders", &options.drawColliders);
    ImGui::Checkbox("Audio Enabled", &options.audioEnabled);
    ImGui::SliderFloat("Master Volume", &options.masterVolume, 0.0F, 1.0F);
    ImGui::Separator();
    ImGui::Checkbox("Pause Execution", &options.isPaused);
    ImGui::SliderFloat("Time Scale", &options.timeScale, 0.0F, 5.0F);
    ImGui::End();
}

void RenderDebugGUISystem::EditorSettingsWindow(EditorPersistence& editorPersistence)
{
    ImGui::Begin("Editor Settings");

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
            char nameBuf[128] = {0};
            if (registry->HasComponent<NameComponent>(selectedEntity))
            {
                const auto& nameComp = registry->GetComponent<NameComponent>(selectedEntity);
                snprintf(nameBuf, sizeof(nameBuf), "%s", nameComp.name.c_str());
            }
            if (ImGui::InputText("Name", nameBuf, sizeof(nameBuf)))
            {
                registry->AddComponent(selectedEntity, NameComponent(std::string(nameBuf)));
            }
        }
        ImGui::Separator();

        const bool hasAnyTransform = registry->HasComponent<PositionComponent>(selectedEntity) ||
            registry->HasComponent<ScaleComponent>(selectedEntity) ||
            registry->HasComponent<RotationComponent>(selectedEntity) ||
            registry->HasComponent<GlobalTransformComponent>(selectedEntity);
        if (hasAnyTransform)
        {
            if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
            {
                if (registry->HasComponent<PositionComponent>(selectedEntity))
                {
                    auto& position = registry->GetComponent<PositionComponent>(selectedEntity);
                    ImGui::DragFloat2("Position", &position.value.x);
                }
                if (registry->HasComponent<ScaleComponent>(selectedEntity))
                {
                    auto& scale = registry->GetComponent<ScaleComponent>(selectedEntity);
                    ImGui::DragFloat2("Scale", &scale.value.x);
                }
                if (registry->HasComponent<RotationComponent>(selectedEntity))
                {
                    auto& rotationComp = registry->GetComponent<RotationComponent>(selectedEntity);
                    auto rotation = static_cast<float>(rotationComp.value);
                    if (ImGui::DragFloat("Rotation", &rotation))
                    {
                        rotationComp.value = rotation;
                    }
                }
                if (registry->HasComponent<GlobalTransformComponent>(selectedEntity))
                {
                    const auto& global = registry->GetComponent<GlobalTransformComponent>(selectedEntity);
                    ImGui::Text("Global Pos: %.2f, %.2f", global.position.x, global.position.y);
                    ImGui::Text("Global Scale: %.2f, %.2f", global.scale.x, global.scale.y);
                    ImGui::Text("Global Rot: %.2f", global.rotation);
                }
            }
        }
        if (registry->HasComponent<HealthComponent>(selectedEntity))
        {
            if (ImGui::CollapsingHeader("Health", ImGuiTreeNodeFlags_DefaultOpen))
            {
                auto& health = registry->GetComponent<HealthComponent>(selectedEntity);
                ImGui::SliderInt("HP", &health.currentHealth, 0, health.maxHealth);
            }
        }
        if (registry->HasComponent<SpriteComponent>(selectedEntity))
        {
            if (ImGui::CollapsingHeader("Sprite", ImGuiTreeNodeFlags_DefaultOpen))
            {
                auto& sprite = registry->GetComponent<SpriteComponent>(selectedEntity);
                ImGui::DragInt("Layer", &sprite.layer);
                ImGui::Text("Asset ID: %s", sprite.assetId.c_str());
                ImGui::DragFloat("Width", &sprite.width);
                ImGui::DragFloat("Height", &sprite.height);
                ImGui::Checkbox("Fixed", &sprite.isFixed);
                auto& assetManager = registry->Get<AssetManager>();
                if (auto* tex = assetManager.GetTexture(sprite.assetId); tex != nullptr)
                {
                    ImGui::Image(reinterpret_cast<ImTextureID>(tex), ImVec2(64, 64));
                }
            }
        }
        if (registry->HasComponent<RigidBodyComponent>(selectedEntity))
        {
            if (ImGui::CollapsingHeader("RigidBody", ImGuiTreeNodeFlags_DefaultOpen))
            {
                auto& rb = registry->GetComponent<RigidBodyComponent>(selectedEntity);
                ImGui::DragFloat2("Velocity", &rb.velocity.x, 1.0f);
            }
        }
        if (registry->HasComponent<BoxColliderComponent>(selectedEntity))
        {
            if (ImGui::CollapsingHeader("BoxCollider", ImGuiTreeNodeFlags_DefaultOpen))
            {
                auto& bc = registry->GetComponent<BoxColliderComponent>(selectedEntity);
                ImGui::DragInt("Width", &bc.width);
                ImGui::DragInt("Height", &bc.height);
                ImGui::DragFloat2("Offset", &bc.offset.x, 1.0f);
            }
        }
        if (registry->HasComponent<AnimationComponent>(selectedEntity))
        {
            if (ImGui::CollapsingHeader("Animation", ImGuiTreeNodeFlags_DefaultOpen))
            {
                auto& anim = registry->GetComponent<AnimationComponent>(selectedEntity);
                ImGui::Text("Num Frames: %d", anim.numFrames);
                ImGui::SliderInt("Current Frame", &anim.currentFrame, 1, anim.numFrames);
                ImGui::DragInt("Frame Speed (ms)", &anim.frameRateSpeed);
                ImGui::Checkbox("Looping", &anim.shouldLoop);
            }
        }
        if (registry->HasComponent<ProjectileEmitterComponent>(selectedEntity))
        {
            if (ImGui::CollapsingHeader("ProjectileEmitter", ImGuiTreeNodeFlags_DefaultOpen))
            {
                auto& pe = registry->GetComponent<ProjectileEmitterComponent>(selectedEntity);
                ImGui::DragFloat2("Velocity", &pe.velocity.x, 1.0f);
                ImGui::DragFloat("Frequency (s)", &pe.frequency, 0.1f);
                ImGui::DragFloat("Duration (s)", &pe.duration, 0.1f);
                ImGui::DragInt("Damage", &pe.damage);
                char projNameBuf[128] = {0};
                snprintf(projNameBuf, sizeof(projNameBuf), "%s", pe.projectileName.c_str());
                if (ImGui::InputText("Projectile Name", projNameBuf, sizeof(projNameBuf)))
                {
                    pe.projectileName = projNameBuf;
                }
            }
        }
        if (registry->HasComponent<TextLabelComponent>(selectedEntity))
        {
            if (ImGui::CollapsingHeader("TextLabel", ImGuiTreeNodeFlags_DefaultOpen))
            {
                auto& tl = registry->GetComponent<TextLabelComponent>(selectedEntity);
                char buf[256];
                snprintf(buf, sizeof(buf), "%s", tl.text.c_str());
                if (ImGui::InputText("Text", buf, sizeof(buf))) tl.text = buf;
                ImGui::Text("Font ID: %s", tl.fontId.c_str());
                float col[4] = {tl.color.r / 255.0f, tl.color.g / 255.0f, tl.color.b / 255.0f, tl.color.a / 255.0f};
                if (ImGui::ColorEdit4("Color", col))
                {
                    tl.color = {
                        static_cast<Uint8>(col[0] * 255), static_cast<Uint8>(col[1] * 255),
                        static_cast<Uint8>(col[2] * 255), static_cast<Uint8>(col[3] * 255)
                    };
                }
                ImGui::Checkbox("Fixed", &tl.isFixed);
            }
        }
        if (registry->HasComponent<ScriptComponent>(selectedEntity))
        {
            if (ImGui::CollapsingHeader("Script", ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::Text("Has Lua script table and callbacks.");
            }
        }
        if (registry->HasComponent<CameraFollowComponent>(selectedEntity))
        {
            if (ImGui::CollapsingHeader("CameraFollow", ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::Text("Entity is being followed by the camera.");
            }
        }
        if (registry->HasComponent<EntityMaskComponent>(selectedEntity))
        {
            if (ImGui::CollapsingHeader("EntityMask", ImGuiTreeNodeFlags_DefaultOpen))
            {
                auto& em = registry->GetComponent<EntityMaskComponent>(selectedEntity);
                ImGui::Text("Mask: %s", em.mask.to_string().c_str());
            }
        }
        if (registry->HasComponent<ProjectileComponent>(selectedEntity))
        {
            if (ImGui::CollapsingHeader("Projectile", ImGuiTreeNodeFlags_DefaultOpen))
            {
                auto& pc = registry->GetComponent<ProjectileComponent>(selectedEntity);
                ImGui::DragInt("Damage", &pc.damage);
                ImGui::Text("Timer: %.2f / %.2f", pc.timer, pc.duration);
            }
        }
        if (registry->HasComponent<SquarePrimitiveComponent>(selectedEntity))
        {
            if (ImGui::CollapsingHeader("SquarePrimitive", ImGuiTreeNodeFlags_DefaultOpen))
            {
                auto& sp = registry->GetComponent<SquarePrimitiveComponent>(selectedEntity);
                ImGui::DragFloat2("Position", &sp.position.x);
                ImGui::DragInt("Layer", &sp.layer);
                ImGui::DragFloat("Width", &sp.width);
                ImGui::DragFloat("Height", &sp.height);
                float col[4] = {sp.color.r / 255.0f, sp.color.g / 255.0f, sp.color.b / 255.0f, sp.color.a / 255.0f};
                if (ImGui::ColorEdit4("Color", col))
                {
                    sp.color = {
                        static_cast<Uint8>(col[0] * 255), static_cast<Uint8>(col[1] * 255),
                        static_cast<Uint8>(col[2] * 255), static_cast<Uint8>(col[3] * 255)
                    };
                }
                ImGui::Checkbox("Fixed", &sp.isFixed);
            }
        }
        if (registry->HasComponent<UIButtonComponent>(selectedEntity))
        {
            if (ImGui::CollapsingHeader("UIButton", ImGuiTreeNodeFlags_DefaultOpen))
            {
                auto& ub = registry->GetComponent<UIButtonComponent>(selectedEntity);
                ImGui::Checkbox("Active", &ub.isActive);
            }
        }

        ImGui::Separator();
        if (ImGui::BeginCombo("Add Component", "Select..."))
        {
            if (!registry->HasComponent<PositionComponent>(selectedEntity) && ImGui::Selectable("Position"))
                registry->AddComponent(selectedEntity, PositionComponent());
            if (!registry->HasComponent<ScaleComponent>(selectedEntity) && ImGui::Selectable("Scale"))
                registry->AddComponent(selectedEntity, ScaleComponent());
            if (!registry->HasComponent<RotationComponent>(selectedEntity) && ImGui::Selectable("Rotation"))
                registry->AddComponent(selectedEntity, RotationComponent());
            if (!registry->HasComponent<GlobalTransformComponent>(selectedEntity) && ImGui::Selectable("Global Transform"))
                registry->AddComponent(selectedEntity, GlobalTransformComponent{});
            if (!registry->HasComponent<RigidBodyComponent>(selectedEntity) && ImGui::Selectable("RigidBody"))
                registry->AddComponent(selectedEntity, RigidBodyComponent());
            if (!registry->HasComponent<SpriteComponent>(selectedEntity) && ImGui::Selectable("Sprite"))
                registry->AddComponent(selectedEntity, SpriteComponent());
            if (!registry->HasComponent<BoxColliderComponent>(selectedEntity) && ImGui::Selectable("BoxCollider"))
                registry->AddComponent(selectedEntity, BoxColliderComponent());
            if (!registry->HasComponent<AnimationComponent>(selectedEntity) && ImGui::Selectable("Animation"))
                registry->AddComponent(selectedEntity, AnimationComponent());
            if (!registry->HasComponent<HealthComponent>(selectedEntity) && ImGui::Selectable("Health"))
                registry->AddComponent(selectedEntity, HealthComponent(100));
            if (!registry->HasComponent<CameraFollowComponent>(selectedEntity) && ImGui::Selectable("CameraFollow"))
                registry->AddComponent(selectedEntity, CameraFollowComponent());
            if (!registry->HasComponent<ProjectileEmitterComponent>(selectedEntity) && ImGui::Selectable(
                "ProjectileEmitter"))
                registry->AddComponent(selectedEntity, ProjectileEmitterComponent());
            if (!registry->HasComponent<TextLabelComponent>(selectedEntity) && ImGui::Selectable("TextLabel"))
                registry->AddComponent(selectedEntity, TextLabelComponent());
            if (!registry->HasComponent<UIButtonComponent>(selectedEntity) && ImGui::Selectable("UIButton"))
                registry->AddComponent(selectedEntity, UIButtonComponent());
            if (!registry->HasComponent<SquarePrimitiveComponent>(selectedEntity) && ImGui::Selectable(
                "SquarePrimitive"))
                registry->AddComponent(selectedEntity, SquarePrimitiveComponent());
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
    static char projectPath[256] = "";
    static std::string lastKnownProjectPath;
    if (lastKnownProjectPath != editorPersistence.lastProjectPath)
    {
        snprintf(projectPath, sizeof(projectPath), "%s", editorPersistence.lastProjectPath.c_str());
        lastKnownProjectPath = editorPersistence.lastProjectPath;
    }

    ImGui::Text("Enter Project Path:");
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - 40);
    ImGui::InputText("##path", projectPath, sizeof(projectPath));
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
            snprintf(projectPath, sizeof(projectPath), "%s", editorPersistence.lastProjectPath.c_str());
    }
    ImGui::End();
}

void RenderDebugGUISystem::SceneWindow(SDL_Texture* gameTexture, bool* p_open)
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
                ImVec2 imagePos =
                    ImVec2((viewportPanelSize.x - imageSize.x) * 0.5f, (viewportPanelSize.y - imageSize.y) * 0.5f);
                ImGui::SetCursorPos(imagePos);
                ImGui::Image(reinterpret_cast<ImTextureID>(gameTexture), imageSize);
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

    if (reclaimFocus)
    {
        ImGui::SetKeyboardFocusHere(-1);
        reclaimFocus = false;
    }
    else if (ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) && !ImGui::IsAnyItemActive() &&
        !ImGui::IsMouseClicked(0))
    {
        ImGui::SetKeyboardFocusHere(-1);
    }

    ImGui::End();
}
#endif
#endif
