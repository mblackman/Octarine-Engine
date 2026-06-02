#pragma once

#ifdef OCTARINE_WITH_EDITOR

namespace sol {
class state;
}

namespace octarine::editor::panels {

/// Lua Console window: scrollback of the engine log with command input + Up/Down history. Submitted
/// commands run against the live sol::state.
void DrawLuaConsoleWindow(sol::state& lua);

}  // namespace octarine::editor::panels

#endif  // OCTARINE_WITH_EDITOR
