#pragma once

#include <sol/sol.hpp>

#include "Components/NameComponent.h"
#include "Lua/Bindings/LuaBinding.h"

template <>
struct LuaBinding<NameComponent> {
  static constexpr const char* kLuaKey = "name";
  static constexpr const char* kUsertypeName = "name_component";

  // Accepts a bare string ( components.name = "foo" ) or a table ( components.name = { value = "foo" } ).
  static NameComponent fromLua(const sol::object& data) {
    if (data.is<std::string>()) return NameComponent(data.as<std::string>());
    if (data.is<sol::table>()) {
      const sol::table tbl = data.as<sol::table>();
      const auto value = tbl.get<sol::object>("value");
      if (value.is<std::string>()) return NameComponent(value.as<std::string>());
      const auto nameField = tbl.get<sol::object>("name");
      if (nameField.is<std::string>()) return NameComponent(nameField.as<std::string>());
    }
    return {};
  }

  static void bindUsertype(sol::state& lua) {
    lua.new_usertype<NameComponent>(kUsertypeName, "name", &NameComponent::name);
  }
};
