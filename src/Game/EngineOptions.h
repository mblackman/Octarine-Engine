#pragma once

struct EngineOptions {
  bool showDebugGUI = false;
  bool drawColliders = false;
  bool showFpsCounter = true;
  bool showEntityInfo = true;
  bool showImGuiDemoWindow = false;
  bool logInputEvents = false;
  bool audioEnabled = true;
  float masterVolume = 1.0F;
  int audioTrackPoolInitial = 32;
  int audioTrackPoolMax = 128;
  bool isPaused = false;
  float timeScale = 1.0F;
  bool stepFrame = false;
};
