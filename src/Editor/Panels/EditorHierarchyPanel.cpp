#include "Editor/Panels/EditorHierarchyPanel.h"

#ifdef OCTARINE_WITH_EDITOR

#include <string>

#include "Components/NameComponent.h"
#include "ECS/Registry.h"
#include "Editor/Inspectors/ComponentInspectorRegistry.h"
#include "Editor/Inspectors/InspectorWidgets.h"
#include "imgui.h"

namespace octarine::editor::panels {

void DrawHierarchyWindow(Registry* registry) {
  ImGui::Begin("Hierarchy / Entity Inspector");
  const auto entities = registry->GetUserEntities();
  ImGui::Text("Total User Entities: %zu", entities.size());
  if (ImGui::Button("Create Entity")) {
    registry->CreateEntity();
  }

  static Entity selectedEntity{UINT64_MAX};
  static ImGuiTextFilter entityFilter;
  entityFilter.Draw("##filter", 140);

  ImGui::BeginChild("EntityList", ImVec2(150, 0), true);
  for (const auto& entity : entities) {
    std::string label;
    if (registry->HasComponent<NameComponent>(entity)) {
      label = registry->GetComponent<NameComponent>(entity).name;
    }
    if (label.empty()) label = "Entity " + std::to_string(entity.id);
    if (!entityFilter.PassFilter(label.c_str())) continue;
    if (ImGui::Selectable(label.c_str(), selectedEntity == entity)) {
      selectedEntity = entity;
    }
  }
  ImGui::EndChild();

  ImGui::SameLine();

  ImGui::BeginChild("EntityDetails", ImVec2(0, 0), true);
  if (registry->IsAlive(selectedEntity)) {
    ImGui::Text("Selected Entity ID: %u", selectedEntity.GetId());
    ImGui::SameLine();
    if (ImGui::Button("Destroy Entity")) {
      registry->BlamEntity(selectedEntity);
      selectedEntity = Entity{UINT64_MAX};
    }
    if (registry->IsAlive(selectedEntity)) {
      std::string name;
      if (registry->HasComponent<NameComponent>(selectedEntity)) {
        name = registry->GetComponent<NameComponent>(selectedEntity).name;
      }
      if (octarine::editor::inspectors::InputTextString("Name", name)) {
        registry->AddComponent(selectedEntity, NameComponent(name));
      }
    }
    ImGui::Separator();

    // Per-component inspector blocks are driven by ComponentInspectorRegistry — each entry
    // renders its own collapsing header when the entity has that component.
    for (const auto& inspector : ComponentInspectorRegistry::all()) {
      inspector.draw(registry, selectedEntity);
    }

    ImGui::Separator();
    if (ImGui::BeginCombo("Add Component", "Select...")) {
      for (const auto& inspector : ComponentInspectorRegistry::all()) {
        if (inspector.addDefault && !inspector.has(registry, selectedEntity) &&
            ImGui::Selectable(inspector.displayName.c_str())) {
          inspector.addDefault(registry, selectedEntity);
        }
      }
      ImGui::EndCombo();
    }
  } else {
    ImGui::Text("No entity selected or entity destroyed.");
  }
  ImGui::EndChild();
  ImGui::End();
}

}  // namespace octarine::editor::panels

#endif  // OCTARINE_WITH_EDITOR
