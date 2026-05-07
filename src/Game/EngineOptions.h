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
};