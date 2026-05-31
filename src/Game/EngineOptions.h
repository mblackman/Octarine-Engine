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
  // Spatial sources beyond this radius from the listener are halted by AudioCullingSystem
  // (track returned to pool, playback offset stashed on the source for clean resume on
  // re-entry). Non-spatial sources ignore this cap.
  float audioListenerRadius = 1500.0F;
  bool isPaused = false;
  float timeScale = 1.0F;
  bool stepFrame = false;
  // When true, a scene whose asset references fail validation aborts the load (dev gate). Off by
  // default so players never hard-fail; the bake step is the CI enforcement point.
  bool assetValidationFatal = false;
};
