#include "Editor/Panels/EditorSceneWindow.h"

#ifdef OCTARINE_WITH_EDITOR

#include "Components/ViewportInfo.h"
#include "Game/Game.h"
#include "imgui.h"

namespace octarine::editor::panels {

void DrawSceneWindow(Game* game, SDL_Texture* gameTexture, bool* p_open) {
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
  if (ImGui::Begin("Scene View", p_open)) {
    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    if (gameTexture) {
      float texW = 0, texH = 0;
      SDL_GetTextureSize(gameTexture, &texW, &texH);
      if (texW > 0 && texH > 0) {
        float aspectRatio = texW / texH;
        float targetW = viewportPanelSize.x;
        float targetH = targetW / aspectRatio;
        if (targetH > viewportPanelSize.y) {
          targetH = viewportPanelSize.y;
          targetW = targetH * aspectRatio;
        }
        ImVec2 imageSize(targetW, targetH);

        // Center the image within the available content area.
        float offsetX = (viewportPanelSize.x - imageSize.x) * 0.5f;
        float offsetY = (viewportPanelSize.y - imageSize.y) * 0.5f;
        ImVec2 contentStartPos = ImGui::GetCursorPos();
        ImGui::SetCursorPos(ImVec2(contentStartPos.x + offsetX, contentStartPos.y + offsetY));

        // Record the image's screen rect (relative to the main viewport origin) so input
        // systems can map window-space cursor coordinates into the shrunken game viewport.
        ImVec2 imageScreenPos = ImGui::GetCursorScreenPos();
        ImVec2 windowPos = ImGui::GetMainViewport()->Pos;
        auto& viewportInfo = game->GetRegistry()->Get<ViewportInfo>();
        viewportInfo.x = imageScreenPos.x - windowPos.x;
        viewportInfo.y = imageScreenPos.y - windowPos.y;
        viewportInfo.width = imageSize.x;
        viewportInfo.height = imageSize.y;

        ImGui::Image(reinterpret_cast<ImTextureID>(gameTexture), imageSize);
        viewportInfo.isHovered = ImGui::IsItemHovered();
        viewportInfo.isFocused = ImGui::IsWindowFocused();
      }
    }
  }
  ImGui::End();
  ImGui::PopStyleVar();
}

}  // namespace octarine::editor::panels

#endif  // OCTARINE_WITH_EDITOR
