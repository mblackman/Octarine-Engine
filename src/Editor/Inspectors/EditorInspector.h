#pragma once

#ifdef OCTARINE_WITH_EDITOR

#include <optional>

#include "ECS/Entity.h"

class Registry;

// Specialize for each inspectable component to expose it in the Hierarchy /
// Entity Inspector. Mirrors the LuaBinding<T> convention (src/Lua/Bindings/).
// Each specialization MUST provide:
//
//   static constexpr const char* kDisplayName;        // collapsing-header label, e.g. "Health"
//   static void draw(Registry*, Entity, T&);          // header body: ImGui widgets for the live component
//   static std::optional<T> makeDefault();            // value for the "Add Component" combo;
//                                                      // std::nullopt => read-only, not addable
//
// The owning ComponentInspectorRegistry supplies the CollapsingHeader wrapper,
// so draw() renders fields only. draw() receives Registry* for the rare
// inspector that needs a singleton (e.g. Sprite -> AssetManager); it must not
// mutate archetypes of the inspected entity beyond the obvious field edits.
template <typename T>
struct EditorInspector;

#endif  // OCTARINE_WITH_EDITOR
