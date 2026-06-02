#pragma once

#ifdef OCTARINE_WITH_EDITOR

class Registry;

namespace octarine::editor::panels {

/// Hierarchy / Entity Inspector window. Left: filterable entity list + create. Right: the
/// per-component inspector host driven by ComponentInspectorRegistry, plus add/destroy controls
/// for the selected entity.
void DrawHierarchyWindow(Registry* registry);

}  // namespace octarine::editor::panels

#endif  // OCTARINE_WITH_EDITOR
