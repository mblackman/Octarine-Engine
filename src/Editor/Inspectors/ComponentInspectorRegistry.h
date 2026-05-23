#pragma once

#ifdef OCTARINE_WITH_EDITOR

#include <imgui.h>

#include <functional>
#include <string>
#include <vector>

#include "ECS/Entity.h"
#include "ECS/Registry.h"
#include "Editor/Inspectors/EditorInspector.h"

// Editor-only parallel to LuaComponentRegistry: a declarative list of component
// inspectors. Adding a component to the inspector = drop an EditorInspector<T>
// header + one registerComponent<T>() line in RegisterAllInspectors.cpp.
class ComponentInspectorRegistry {
 public:
  struct Entry {
    std::string displayName;
    std::function<bool(Registry*, Entity)> has;
    std::function<void(Registry*, Entity)> draw;        // CollapsingHeader + EditorInspector<T>::draw when present
    std::function<void(Registry*, Entity)> addDefault;  // null when EditorInspector<T>::makeDefault() == nullopt
  };

  template <typename T>
  static void registerComponent() {
    using I = EditorInspector<T>;
    Entry entry;
    entry.displayName = I::kDisplayName;
    entry.has = [](Registry* r, Entity e) { return r->HasComponent<T>(e); };
    entry.draw = [](Registry* r, Entity e) {
      if (!r->HasComponent<T>(e)) return;
      if (ImGui::CollapsingHeader(I::kDisplayName, ImGuiTreeNodeFlags_DefaultOpen)) {
        I::draw(r, e, r->GetComponent<T>(e));
      }
    };
    if (I::makeDefault().has_value()) {
      entry.addDefault = [](Registry* r, Entity e) {
        if (auto value = I::makeDefault()) r->AddComponent(e, *value);
      };
    }
    entries().push_back(std::move(entry));
  }

  static const std::vector<Entry>& all() { return entries(); }

  static void clear() { entries().clear(); }

 private:
  static std::vector<Entry>& entries() {
    static std::vector<Entry> instance;
    return instance;
  }
};

#endif  // OCTARINE_WITH_EDITOR
