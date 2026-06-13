#pragma once

#include <sol/sol.hpp>
#include <string>

#include "Components/TextLabelComponent.h"
#include "Lua/Bindings/LuaBinding.h"

template <>
struct LuaBinding<TextLabelComponent> {
  static constexpr const char* kLuaKey = "text_label";
  static constexpr const char* kUsertypeName = "text_label_component";

  // "left"/"top" → Start, "center" → Center, "right"/"bottom" → End. Unknown values fall back to
  // Start (the historical top-left placement).
  static TextAlign ParseAlign(const std::string& value) {
    if (value == "center") return TextAlign::Center;
    if (value == "right" || value == "bottom" || value == "end") return TextAlign::End;
    return TextAlign::Start;
  }

  static TextLabelComponent fromLua(const sol::object& data) {
    const auto t = data.as<sol::table>();
    using namespace LuaComponentHelpers;
    const glm::vec2 offsetPosition = SafeGetVec2(t, "position");
    const int layer = SafeGetOptionalValue<int>(t, "layer", 1);
    const std::string text = t["text"].get<std::string>();
    const std::string fontId = t["font_id"].get<std::string>();
    const octarine::Color color = SafeGetColor(t, "color");
    const bool isFixed = SafeGetOptionalValue<bool>(t, "is_fixed", true);
    const TextAlign hAlign = ParseAlign(SafeGetOptionalValue<std::string>(t, "align", "left"));
    const TextAlign vAlign = ParseAlign(SafeGetOptionalValue<std::string>(t, "valign", "top"));
    return TextLabelComponent(offsetPosition, layer, text, fontId, color, isFixed, hAlign, vAlign);
  }

  static void bindUsertype(sol::state& lua) {
    lua.new_usertype<TextLabelComponent>(kUsertypeName, "text", &TextLabelComponent::text, "color",
                                         &TextLabelComponent::color, "layer", &TextLabelComponent::layer, "position",
                                         &TextLabelComponent::position, "is_fixed", &TextLabelComponent::isFixed,
                                         "h_align", &TextLabelComponent::hAlign, "v_align",
                                         &TextLabelComponent::vAlign);
  }
};
