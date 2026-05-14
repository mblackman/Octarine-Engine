#pragma once

struct EngineOptions {
  bool showDebugGUI = false;
  bool drawColliders = false;
#ifdef OCTARINE_WITH_IMGUI
  bool showFpsCounter = true;
  bool showEntityInfo = true;
  bool showProfiler = false;
#endif
#ifdef OCTARINE_WITH_EDITOR
  bool showImGuiDemoWindow = false;
  bool showHierarchy = false;
  bool showAssetBrowser = false;
  bool showLuaConsole = false;
  bool showSceneWindow = true;
  bool showSceneManagement = false;
#endif
  bool audioEnabled = true;
  float masterVolume = 1.0F;
  bool isPaused = false;
  float timeScale = 1.0F;
  bool stepFrame = false;

#ifdef OCTARINE_WITH_EDITOR
  // Editor persistence
  float editorFontSize = 0.0F;
  int editorStyleIndex = 0;
#endif
  std::string lastProjectPath;
  std::string currentScenePath;
};