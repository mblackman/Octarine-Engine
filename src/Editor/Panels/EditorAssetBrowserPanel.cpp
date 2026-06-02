#include "Editor/Panels/EditorAssetBrowserPanel.h"

#ifdef OCTARINE_WITH_EDITOR

#include "AssetManager/AssetManager.h"
#include "ECS/Registry.h"
#include "imgui.h"

namespace octarine::editor::panels {

void DrawAssetBrowserWindow(Registry* registry) {
  ImGui::Begin("Asset Browser");
  auto& assetManager = registry->Get<AssetManager>();
  if (ImGui::CollapsingHeader("Textures", ImGuiTreeNodeFlags_DefaultOpen)) {
    const auto& textures = assetManager.GetTextures();
    ImGui::Text("Loaded: %zu", textures.size());
    for (const auto& [id, texture] : textures) ImGui::BulletText("%s", id.c_str());
  }
  if (ImGui::CollapsingHeader("Fonts", ImGuiTreeNodeFlags_DefaultOpen)) {
    const auto& fonts = assetManager.GetFonts();
    ImGui::Text("Loaded: %zu", fonts.size());
    for (const auto& [id, font] : fonts) ImGui::BulletText("%s", id.c_str());
  }
  if (ImGui::CollapsingHeader("Audio Clips", ImGuiTreeNodeFlags_DefaultOpen)) {
    const auto& audioClips = assetManager.GetAudioClips();
    ImGui::Text("Loaded: %zu", audioClips.size());
    for (const auto& [id, clip] : audioClips) ImGui::BulletText("%s", id.c_str());
  }
  ImGui::End();
}

}  // namespace octarine::editor::panels

#endif  // OCTARINE_WITH_EDITOR
