#pragma once

struct EngineOptions {
  bool showDebugGUI = false;
  bool drawColliders = false;
  bool showFpsCounter = true;
  bool showEntityInfo = true;
  bool showProfiler = false;
  bool showImGuiDemoWindow = false;
  bool showHierarchy = false;
  bool showAssetBrowser = false;
  bool showLuaConsole = false;
  bool showSceneWindow = true;
  bool showSceneManagement = false;
  bool audioEnabled = true;
  float masterVolume = 1.0F;
  bool isPaused = false;
  float timeScale = 1.0F;
  bool stepFrame = false;

  // Editor persistence
  float editorFontSize = 0.0F;
  int editorStyleIndex = 0;
  std::string lastProjectPath;
  std::string currentScenePath;
};
