#pragma once

#include <sol/sol.hpp>

#include "Components/UIRectComponent.h"
#include "Lua/Bindings/LuaBinding.h"

template <>
struct LuaBinding<UIRectComponent> {
  static constexpr const char* kLuaKey = "ui_rect";
  static constexpr const char* kUsertypeName = "ui_rect_component";

  // UIRectComponent is written by UILayoutSystem; fromLua is a stub for registry accessor symmetry.
  static UIRectComponent fromLua(const sol::object&) { return UIRectComponent{}; }

  static void bindUsertype(sol::state& lua) {
    lua.new_usertype<UIRectComponent>(
        kUsertypeName, "left", sol::property([](const UIRectComponent& r) { return r.left; }), "top",
        sol::property([](const UIRectComponent& r) { return r.top; }), "right",
        sol::property([](const UIRectComponent& r) { return r.right; }), "bottom",
        sol::property([](const UIRectComponent& r) { return r.bottom; }), "layer",
        sol::property([](const UIRectComponent& r) { return r.layer; }), "width",
        sol::property(&UIRectComponent::Width), "height", sol::property(&UIRectComponent::Height), "center",
        sol::property(&UIRectComponent::Center));
  }
};
