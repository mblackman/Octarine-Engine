#pragma once

#ifdef OCTARINE_WITH_EDITOR

#include <SDL3/SDL.h>
#include <imgui.h>

#include <string>

// Shared ImGui helpers for component inspectors. Keeps the per-component
// EditorInspector<T>::draw bodies free of repeated boilerplate.
namespace octarine::editor::inspectors {

namespace detail {
struct InputTextStringUserData {
  std::string* str;
};

inline int InputTextStringResize(ImGuiInputTextCallbackData* data) {
  if (data->EventFlag == ImGuiInputTextFlags_CallbackResize) {
    auto* userData = static_cast<InputTextStringUserData*>(data->UserData);
    std::string* str = userData->str;
    str->resize(static_cast<size_t>(data->BufTextLen));
    data->Buf = str->data();
  }
  return 0;
}
}  // namespace detail

/// InputText bound directly to a std::string (resize-callback backed, no fixed
/// cap). Replaces the snprintf-into-char[] + write-back dance.
inline bool InputTextString(const char* label, std::string& str, ImGuiInputTextFlags flags = 0) {
  flags |= ImGuiInputTextFlags_CallbackResize;
  detail::InputTextStringUserData userData{&str};
  return ImGui::InputText(label, str.data(), str.capacity() + 1, flags, detail::InputTextStringResize, &userData);
}

/// ColorEdit4 bound to an SDL_Color (0-255). Handles the float[4] <-> Uint8
/// conversion both ways.
inline void DrawColorEdit(const char* label, SDL_Color& color) {
  float col[4] = {color.r / 255.0F, color.g / 255.0F, color.b / 255.0F, color.a / 255.0F};
  if (ImGui::ColorEdit4(label, col)) {
    color = {static_cast<Uint8>(col[0] * 255), static_cast<Uint8>(col[1] * 255), static_cast<Uint8>(col[2] * 255),
             static_cast<Uint8>(col[3] * 255)};
  }
}

}  // namespace octarine::editor::inspectors

#endif  // OCTARINE_WITH_EDITOR
