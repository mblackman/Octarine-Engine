#pragma once

#ifdef OCTARINE_WITH_EDITOR

namespace octarine::editor::panels {

/// Signing Settings window: stores Android-release signing credentials via the OS secret backend
/// (DPAPI / Keychain). Falls back to an env-var-only notice on platforms without secret storage.
void DrawSigningSettingsWindow(bool* p_open);

}  // namespace octarine::editor::panels

#endif  // OCTARINE_WITH_EDITOR
