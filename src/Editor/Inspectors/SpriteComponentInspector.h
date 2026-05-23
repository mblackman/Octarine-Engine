#pragma once

#ifdef OCTARINE_WITH_EDITOR

#include <imgui.h>

#include <optional>

#include "AssetManager/AssetManager.h"
#include "Components/SpriteComponent.h"
#include "ECS/Registry.h"
#include "Editor/Inspectors/EditorInspector.h"

template <>
struct EditorInspector<SpriteComponent> {
  static constexpr const char* kDisplayName = "Sprite";
  static void draw(Registry* registry, Entity /*entity*/, SpriteComponent& sprite) {
    ImGui::DragInt("Layer", &sprite.layer);
    ImGui::Text("Asset ID: %s", sprite.assetId.c_str());
    ImGui::DragFloat("Width", &sprite.width);
    ImGui::DragFloat("Height", &sprite.height);
    ImGui::Checkbox("Fixed", &sprite.isFixed);
    auto& assetManager = registry->Get<AssetManager>();
    if (auto* tex = assetManager.GetTexture(sprite.assetId); tex != nullptr) {
      ImGui::Image(reinterpret_cast<ImTextureID>(tex), ImVec2(64, 64));
    }
  }
  static std::optional<SpriteComponent> makeDefault() { return SpriteComponent{}; }
};

#endif  // OCTARINE_WITH_EDITOR
