#pragma once

#ifdef OCTARINE_WITH_EDITOR

#include <array>
#include <string>
#include <utility>

// Editor-only persistent state. Lives outside GameConfig so the player build
// neither carries these fields nor reads/writes their files.
//
// Two stores:
//   * Global  — SDL pref dir / "editor_settings.ini": which project to reopen,
//               font size, theme. Survives across projects.
//   * Project — "<project>/editor_prefs.ini": per-project window layout,
//               last-open scene. Travels with the project.
struct EditorPersistence {
  // Global
  std::string lastProjectPath;
  float editorFontSize = 0.0F;
  int editorStyleIndex = 0;
  // Audio mute + master volume. Persisted globally so they survive across projects (and with no
  // project loaded). Applied to the live EngineOptions at startup; EngineOptions stays the source
  // AudioSystem reads, these are its persisted shadow.
  bool audioMuted = false;
  float masterVolume = 1.0F;

  // Transient: set when the editor wants the font atlas rebuilt outside the
  // active ImGui frame. Not serialized.
  bool fontRebuildPending = false;

  // Transient: set when a layout preset should be loaded at the start of the next frame.
  // Contains the preset name to load.
  std::string pendingLayoutLoad;

  // Per-project
  std::string currentScenePath;
  bool showProfiler = false;
  bool showHierarchy = false;
  bool showAssetBrowser = false;
  bool showLuaConsole = false;
  bool showSceneWindow = true;
  bool showSceneManagement = false;
  bool showEngineOptions = true;
  bool showEditorSettings = true;
  bool showPlayerOutput = false;

  // Single source of truth for persisted window-visibility flags. Both the project-prefs
  // serializer (EditorPersistence.cpp) and the layout-preset serializer (EditorLayoutPresets.cpp)
  // iterate this table, so a new window = one entry here.
  using FlagRef = std::pair<const char*, bool EditorPersistence::*>;
  static constexpr std::array<FlagRef, 9> kWindowFlags = {{
      {"showProfiler", &EditorPersistence::showProfiler},
      {"showHierarchy", &EditorPersistence::showHierarchy},
      {"showAssetBrowser", &EditorPersistence::showAssetBrowser},
      {"showLuaConsole", &EditorPersistence::showLuaConsole},
      {"showSceneWindow", &EditorPersistence::showSceneWindow},
      {"showSceneManagement", &EditorPersistence::showSceneManagement},
      {"showEngineOptions", &EditorPersistence::showEngineOptions},
      {"showEditorSettings", &EditorPersistence::showEditorSettings},
      {"showPlayerOutput", &EditorPersistence::showPlayerOutput},
  }};

  void LoadGlobal();
  void SaveGlobal() const;

  void LoadProject(const std::string& projectAssetPath);
  void SaveProject(const std::string& projectAssetPath) const;
};

// Splits "key=value" into key/value, trimming trailing whitespace and CR (handles CRLF files /
// hand edits). Returns false when the line has no '='.
bool ParseIniLine(const std::string& line, std::string& key, std::string& value);

#endif  // OCTARINE_WITH_EDITOR
