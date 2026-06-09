#include "Editor/Panels/EditorImGuiBackend.h"

#ifdef OCTARINE_WITH_EDITOR

#include "General/Fonts/Roboto_Medium.h"
#include "imgui.h"
#include "imgui_impl_sdlrenderer3.h"

namespace octarine::editor::panels {

void ApplyEditorStyle(const int styleIndex, const float fontSizePx) {
  switch (styleIndex) {
    default:
    case 0:
      ImGui::StyleColorsDark();
      break;
    case 1:
      ImGui::StyleColorsLight();
      break;
    case 2:
      ImGui::StyleColorsClassic();
      break;
  }
  ImGuiStyle& style = ImGui::GetStyle();
  style.FrameRounding = 4.0F;
  style.GrabRounding = 4.0F;
  style.WindowRounding = 6.0F;
  style.FramePadding = ImVec2(6, 4);
  style.ItemSpacing = ImVec2(8, 6);
  style.ScrollbarSize = 16.0F;
  style.GrabMinSize = 14.0F;

  const float scale = (fontSizePx > 0.0F) ? (fontSizePx / kBaselineFontSize) : 1.0F;
  if (scale != 1.0F) {
    style.ScaleAllSizes(scale);
  }
}

void RebuildEditorFont(const float sizePx) {
  ImGuiIO& io = ImGui::GetIO();
  io.Fonts->Clear();

  ImFontConfig fontConfig;
  fontConfig.SizePixels = (sizePx > 0.0F) ? sizePx : kBaselineFontSize;
  fontConfig.OversampleH = 3;
  fontConfig.OversampleV = 3;
  fontConfig.PixelSnapH = false;
  // Tell ImGui not to free our static data — it lives in .rodata.
  fontConfig.FontDataOwnedByAtlas = false;

  ImFont* font = io.Fonts->AddFontFromMemoryTTF(const_cast<unsigned char*>(octarine::fonts::kRobotoMediumData),
                                                static_cast<int>(octarine::fonts::kRobotoMediumSize),
                                                fontConfig.SizePixels, &fontConfig);
  if (font == nullptr) {
    // Fallback so the editor still boots if the embedded load fails for any reason.
    io.FontDefault = io.Fonts->AddFontDefault();
  } else {
    io.FontDefault = font;
  }
  io.Fonts->Build();
  ImGui_ImplSDLRenderer3_DestroyDeviceObjects();
  ImGui_ImplSDLRenderer3_CreateDeviceObjects();
}

}  // namespace octarine::editor::panels

#endif  // OCTARINE_WITH_EDITOR
