#pragma once

#include <sol/sol.hpp>
#include <string>
#include <string_view>

#include "Components/ColorGridComponent.h"
#include "General/BlendMode.h"
#include "Lua/Bindings/LuaBinding.h"

template <>
struct LuaBinding<ColorGridComponent> {
  static constexpr const char* kLuaKey = "color_grid";
  static constexpr const char* kUsertypeName = "color_grid_component";

  static ColorGridComponent fromLua(const sol::object& data) {
    const auto t = data.as<sol::table>();
    using namespace LuaComponentHelpers;
    ColorGridComponent grid;
    const auto cellSize = SafeGetOptionalValue<float>(t, "cell_size", 32.0f);
    grid.cellWidth = SafeGetOptionalValue<float>(t, "cell_width", cellSize);
    grid.cellHeight = SafeGetOptionalValue<float>(t, "cell_height", cellSize);
    if (t["color1"].valid()) {
      grid.color1 = SafeGetColor(t, "color1");
    } else if (t["color_a"].valid()) {
      grid.color1 = SafeGetColor(t, "color_a");
    }
    if (t["color2"].valid()) {
      grid.color2 = SafeGetColor(t, "color2");
    } else if (t["color_b"].valid()) {
      grid.color2 = SafeGetColor(t, "color_b");
    }
    grid.layer = SafeGetOptionalValue<int>(t, "layer", 0);
    grid.isFixed = SafeGetOptionalValue<bool>(t, "is_fixed", false);
    grid.offset = SafeGetVec2(t, "offset", 0.0f, 0.0f);
    grid.scrollVelocity = SafeGetVec2(t, "scroll_velocity", 0.0f, 0.0f);
    grid.bounds = SafeGetVec2(t, "bounds", 0.0f, 0.0f);
    grid.blendMode = octarine::BlendModeFromString(SafeGetOptionalValue<std::string>(t, "blend_mode", "blend"));
    return grid;
  }

  static void bindUsertype(sol::state& lua) {
    lua.new_usertype<ColorGridComponent>(
        kUsertypeName, "cell_width", &ColorGridComponent::cellWidth, "cell_height", &ColorGridComponent::cellHeight,
        "cell_size",
        sol::property([](const ColorGridComponent& g) { return g.GetCellSize(); },
                      [](ColorGridComponent& g, const float s) { g.SetCellSize(s); }),
        "color1", &ColorGridComponent::color1, "color2", &ColorGridComponent::color2, "color_a",
        &ColorGridComponent::color1, "color_b", &ColorGridComponent::color2, "offset", &ColorGridComponent::offset,
        "scroll_velocity", &ColorGridComponent::scrollVelocity, "bounds", &ColorGridComponent::bounds, "layer",
        &ColorGridComponent::layer, "is_fixed", &ColorGridComponent::isFixed, "blend_mode",
        sol::property([](const ColorGridComponent& g) { return octarine::ToString(g.blendMode); },
                      [](ColorGridComponent& g, const std::string_view mode) {
                        g.blendMode = octarine::BlendModeFromString(mode, g.blendMode);
                      }));
  }
};
