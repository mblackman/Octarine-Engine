#pragma once

#ifdef OCTARINE_WITH_EDITOR

class Game;

namespace octarine::editor::panels {

/// Draws the two editor modals raised from the toolbar this frame: the Export Build dialog (target
/// + version fields + validation) and the Save Layout Preset dialog. The `open*` flags trigger the
/// matching ImGui::OpenPopup; both popups are otherwise persistent ImGui modals.
void DrawBuildDialogs(Game* game, bool openSaveLayoutModal, bool openExportBuildModal);

}  // namespace octarine::editor::panels

#endif  // OCTARINE_WITH_EDITOR
