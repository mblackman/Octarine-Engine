#pragma once

#ifdef OCTARINE_WITH_EDITOR

class Game;

namespace octarine::editor::panels {

/// Devices panel — unified surface for Play / Deploy / Hot-push across all attached targets:
/// Local host, connected Android devices (via adb), and manually-added Remote TCP dev-listen
/// servers. Auto-refreshes the adb device list every 2 seconds.
void DrawDevicesWindow(Game* game, bool* p_open);

}  // namespace octarine::editor::panels

#endif  // OCTARINE_WITH_EDITOR
