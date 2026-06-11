#pragma once

#include <cstdint>

// Screen anchor for the built-in perf overlay. config.ini: PerfOverlayCorner=top-left|top-right|
// bottom-left|bottom-right.
enum class PerfOverlayCorner : std::uint8_t { TopLeft, TopRight, BottomLeft, BottomRight };

// Which metrics the perf overlay draws. config.ini: PerfOverlayMetrics takes a comma-separated
// list of fps|frametime|entities|memory, or all ("both" accepted as a legacy alias).
enum class PerfOverlayMetrics : std::uint8_t {
  Fps = 1 << 0,
  FrameTime = 1 << 1,
  Entities = 1 << 2,
  Memory = 1 << 3,
  All = Fps | FrameTime | Entities | Memory
};

inline PerfOverlayMetrics operator|(PerfOverlayMetrics lhs, PerfOverlayMetrics rhs) {
  return static_cast<PerfOverlayMetrics>(static_cast<std::uint8_t>(lhs) | static_cast<std::uint8_t>(rhs));
}

struct EngineOptions {
  bool showDebugGUI = false;
  bool drawColliders = false;
  bool showFpsCounter = true;
  bool showEntityInfo = true;
  // Built-in renderer FPS + frame-time overlay (no ImGui). Unlike the ImGui FPS window this ships in
  // packaged builds, so it's the player-facing perf readout. Off by default; opt in via the
  // `PerfOverlay` config.ini key, the editor settings toggle, or set_perf_overlay() from Lua.
  bool showPerfOverlay = false;
  // Placement + content of the perf overlay (config.ini only: PerfOverlayCorner=, PerfOverlayMetrics=).
  PerfOverlayCorner perfOverlayCorner = PerfOverlayCorner::TopLeft;
  PerfOverlayMetrics perfOverlayMetrics = PerfOverlayMetrics::All;
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
  // Lua script hot reload. Compiled out under OCTARINE_SHIPPED; in dev/editor builds this is the
  // runtime toggle. Poll cadence is mtime-based, single-threaded, on the main loop.
  bool hotReloadEnabled = true;
  float hotReloadPollSeconds = 0.25F;
};
