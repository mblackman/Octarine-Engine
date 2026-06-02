#pragma once

#ifdef OCTARINE_WITH_EDITOR

struct EngineOptions;
struct EditorPersistence;

namespace octarine::editor::panels {

/// Engine Options window: runtime toggles (overlays, audio, pause/step, time scale). Window
/// show/hide lives in the Windows menu, not here.
void DrawEngineOptionsWindow(EngineOptions& options, bool* p_open);

/// Editor Settings window: appearance (font size, theme). Applies style immediately; defers the
/// font-atlas rebuild to the end of the frame via EditorPersistence::fontRebuildPending.
void DrawEditorSettingsWindow(EditorPersistence& editorPersistence, bool* p_open);

/// Performance Profiler window: tables of accumulated scope timings and perf counters.
void DrawPerformanceProfilerWindow();

}  // namespace octarine::editor::panels

#endif  // OCTARINE_WITH_EDITOR
