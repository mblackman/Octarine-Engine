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
  // When true, a scene whose asset references fail validation aborts the load (dev gate). Off by
  // default so players never hard-fail; the bake step is the CI enforcement point.
  bool assetValidationFatal = false;
  // Lua script hot reload. Compiled out under OCTARINE_SHIPPED; in dev/editor builds this is the
  // runtime toggle. Poll cadence is mtime-based, single-threaded, on the main loop.
  bool hotReloadEnabled = true;
  float hotReloadPollSeconds = 0.25F;
};
