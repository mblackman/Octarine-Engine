#pragma once

#ifdef OCTARINE_WITH_EDITOR

class Game;

namespace octarine::editor::panels {

/// Draws the main menu bar (File / Layout / Windows) and the playback toolbar pinned beneath it
/// (Play / Pause / Step / Stop / Mute / Run Player / Export Build). The out-params are raised when
/// the user requests an action whose modal is drawn later in the frame by DrawBuildDialogs, and
/// `showProjectSelector` is toggled by File > Open Project.
void DrawToolbar(Game* game, bool& showProjectSelector, bool& openSaveLayoutModal, bool& openExportBuildModal);

}  // namespace octarine::editor::panels

#endif  // OCTARINE_WITH_EDITOR
