#pragma once

#ifdef OCTARINE_WITH_EDITOR

namespace octarine::editor::panels {

/// Baseline font size in pixels that matches the unscaled style metrics applied by
/// ApplyEditorStyle. Font sizes larger/smaller than this scale all widget metrics
/// proportionally so the UI stays balanced at any zoom level.
inline constexpr float kBaselineFontSize = 16.0F;

/// Apply one of the three built-in ImGui styles.  0 = Dark, 1 = Light, 2 = Classic.
/// `fontSizePx` is the current font size; widget metrics are scaled relative to
/// kBaselineFontSize. Always re-applies baseline metrics first so repeated calls
/// do not compound.
void ApplyEditorStyle(int styleIndex, float fontSizePx = kBaselineFontSize);

/// Rebuild the ImGui font atlas at `sizePx` using the bundled Roboto TTF.
/// MUST be called outside an active ImGui frame (after ImGui::Render or before
/// the next NewFrame). Also re-uploads the renderer's device texture.
void RebuildEditorFont(float sizePx);

}  // namespace octarine::editor::panels

#endif  // OCTARINE_WITH_EDITOR
