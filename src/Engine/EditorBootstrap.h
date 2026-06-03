#pragma once

#include <string>

class Registry;

// Editor-mode boot/teardown glue, factored out of Game so Game.cpp carries no OCTARINE_WITH_EDITOR
// branches. The real implementations live in EditorBootstrap.cpp under OCTARINE_WITH_EDITOR; player
// builds compile no-op stubs in the same TU. Every entry point is callable unconditionally — the
// feature selection happens here, not at the call site.
namespace engine_bootstrap::editor {

// Set the editor-only Registry singletons (EditorPersistence + PlayerLauncher + ExportBuilder),
// load global editor prefs, and — when `effectivePath` is empty — default it to the last-opened
// project. No-op in player builds.
void InstallSingletons(Registry& registry, std::string& effectivePath);

// After a project config loads from `path`: load its per-project editor prefs, record it as the
// last-opened project, and persist. No-op in player builds.
void OnProjectLoaded(Registry& registry, const std::string& path);

// Apply the editor-global audio prefs (mute + master volume) on top of the per-project
// preferences, which are authoritative for editor sessions. No-op in player builds.
void ApplyAudioPrefs(Registry& registry);

// Whether the session should default into editor mode, and whether Initialize may proceed.
struct StartupModeDecision {
  bool ok = true;  // false → Initialize should abort (a player build was asked for editor mode)
  bool defaultToEditor = false;
};

// Editor builds default to editor mode unless a non-editor startup mode was requested. Player
// builds never default to editor and reject an explicit `--startup-mode editor` (ok=false).
[[nodiscard]] StartupModeDecision DecideStartupMode(const std::string& startupMode);

// Open the editor session paused so the scene sits ready to inspect (the toolbar Play button
// starts it). No-op in player builds.
void PauseForEditorSession(Registry& registry);

// One-time ImGui font/style/layout setup. Editor: the bundled Roboto at the persisted size +
// theme + the default layout on first run. Player-with-ImGui: the built-in default font. Call once
// after EngineRuntime::InitImGui, from inside an OCTARINE_WITH_IMGUI block.
void SetupEditorImGui(Registry& registry);

// Persist editor prefs (audio + per-project window layout) on shutdown. No-op in player builds.
void SaveOnShutdown(Registry& registry);

// Suppress the editor's debug windows for benchmark runs. No-op in player builds.
void DisableBenchOverlays(Registry& registry);

}  // namespace engine_bootstrap::editor
