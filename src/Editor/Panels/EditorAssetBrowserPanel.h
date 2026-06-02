#pragma once

#ifdef OCTARINE_WITH_EDITOR

class Registry;

namespace octarine::editor::panels {

/// Asset Browser window: lists the loaded textures, fonts, and audio clips held by AssetManager.
void DrawAssetBrowserWindow(Registry* registry);

}  // namespace octarine::editor::panels

#endif  // OCTARINE_WITH_EDITOR
