#pragma once

#ifdef OCTARINE_WITH_EDITOR

class Game;

namespace octarine::editor::panels {

/// Player Output window: live status + captured stdout/stderr of the standalone player process
/// spawned by PlayerLauncher.
void DrawPlayerOutputWindow(Game* game, bool* p_open);

/// Export Output window: live status + captured stdout/stderr of the export build subprocess
/// spawned by ExportBuilder.
void DrawExportOutputWindow(Game* game, bool* p_open);

}  // namespace octarine::editor::panels

#endif  // OCTARINE_WITH_EDITOR
