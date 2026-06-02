#pragma once

#ifdef OCTARINE_WITH_EDITOR

class Game;

namespace octarine::editor::panels {

/// Scene Management window: edit/browse the scene script path and load / reload / stop it.
void DrawSceneManagementWindow(Game* game);

/// Project Selector window: pick or browse a project folder. Saving the path requires an engine
/// restart to take effect.
void DrawProjectSelectorWindow(Game* game, bool* p_open);

}  // namespace octarine::editor::panels

#endif  // OCTARINE_WITH_EDITOR
