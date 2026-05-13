#pragma once

struct EngineOptions {
  bool showDebugGUI = false;
  bool drawColliders = false;
  bool showImGuiDemoWindow = false;
  bool showFpsCounter = true;
  bool showEntityInfo = true;
  bool showProfiler = false;
  bool showHierarchy = false;
  bool showAssetBrowser = false;
  bool audioEnabled = true;
  float masterVolume = 1.0F;
  bool isPaused = false;
  float timeScale = 1.0F;
  bool stepFrame = false;

  // Editor UI appearance — persisted in preferences.ini.
  // 0.0 = auto-detect from display DPI; any positive value = explicit pixel size.
  float editorFontSize = 0.0F;
  // 0 = Dark, 1 = Light, 2 = Classic
  int editorStyleIndex = 0;
};