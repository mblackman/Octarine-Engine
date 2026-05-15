#pragma once

#ifdef OCTARINE_WITH_EDITOR

#include <string>

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

  // Transient: set when the editor wants the font atlas rebuilt outside the
  // active ImGui frame. Not serialized.
  bool fontRebuildPending = false;

  // Per-project
  std::string currentScenePath;
  bool showProfiler = false;
  bool showHierarchy = false;
  bool showAssetBrowser = false;
  bool showLuaConsole = false;
  bool showSceneWindow = true;
  bool showSceneManagement = false;

  void LoadGlobal();
  void SaveGlobal() const;

  void LoadProject(const std::string& projectAssetPath);
  void SaveProject(const std::string& projectAssetPath) const;
};

#endif  // OCTARINE_WITH_EDITOR
