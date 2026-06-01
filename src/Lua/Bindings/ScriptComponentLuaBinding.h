#pragma once

#include <sol/sol.hpp>
#include <string>

#include "Components/ScriptComponent.h"
#include "General/Logger.h"
#include "Lua/Bindings/LuaBinding.h"

template <>
struct LuaBinding<ScriptComponent> {
  static constexpr const char* kLuaKey = "script";
  static constexpr const char* kUsertypeName = "script_component";

  static ScriptComponent fromLua(const sol::object& data) {
    const auto spec = data.as<sol::table>();
    using namespace LuaComponentHelpers;

    // File-backed form: `{ source = "scripts/foo.lua" }`. The named file must return a table
    // whose `on_update` / `on_debug_gui` / `data` fields drive the component. Recorded
    // sourcePath flags the component for hot reload.
    const sol::object sourceObj = spec.get<sol::object>("source");
    if (sourceObj.is<std::string>()) {
      return FromSource(spec, sourceObj.as<std::string>());
    }

    // Inline form (status quo): table itself carries on_update / on_debug_gui. sourcePath stays
    // empty so the hot-reload subsystem leaves it alone.
    const sol::protected_function updateFn = SafeGetProtectedFunction(spec, "on_update");
    const sol::protected_function onDebugGuiFn = SafeGetProtectedFunction(spec, "on_debug_gui");
    return ScriptComponent(spec, updateFn, onDebugGuiFn, /*sourcePath=*/"");
  }

  static void bindUsertype(sol::state& lua) {
    lua.new_usertype<ScriptComponent>(kUsertypeName, "data", &ScriptComponent::scriptTable);
  }

 private:
  static ScriptComponent FromSource(const sol::table& spec, const std::string& relPath) {
    sol::state_view lua(spec.lua_state());
    // Explicit .get<>() — implicit conversion from sol::table_proxy to protected_function is
    // ambiguous under GCC's -Wconversion (two viable overloads; ours is the operator T()).
    auto getAssetPath = lua["get_asset_path"].get<sol::protected_function>();
    if (!getAssetPath.valid()) {
      Logger::Error("ScriptComponent: get_asset_path not installed; cannot resolve '" + relPath + "'");
      return ScriptComponent();
    }

    std::string absPath;
    try {
      absPath = getAssetPath(relPath).get<std::string>();
    } catch (const std::exception& ex) {
      Logger::Error("ScriptComponent: get_asset_path('" + relPath + "') failed: " + ex.what());
      return ScriptComponent();
    }

    // protected_function so a syntax error in the file returns an invalid result instead of
    // propagating as a C++ exception (engine compiles without /EHsc).
    auto dofile = lua["dofile"].get<sol::protected_function>();
    if (!dofile.valid()) {
      Logger::Error("ScriptComponent: dofile not installed (Lua setup ordering bug)");
      return ScriptComponent();
    }

    sol::protected_function_result result = dofile(absPath);
    if (!result.valid()) {
      const auto err = result.get<sol::error>();
      Logger::ErrorLua("ScriptComponent dofile '" + absPath + "': " + err.what());
      return ScriptComponent(spec, sol::lua_nil, sol::lua_nil, absPath);
    }

    const auto returned = result.get<sol::object>();
    if (!returned.is<sol::table>()) {
      Logger::Error("ScriptComponent: '" + absPath + "' did not return a table");
      return ScriptComponent(spec, sol::lua_nil, sol::lua_nil, absPath);
    }

    const sol::table loaded = returned.as<sol::table>();
    using namespace LuaComponentHelpers;
    const sol::protected_function updateFn = SafeGetProtectedFunction(loaded, "on_update");
    const sol::protected_function onDebugGuiFn = SafeGetProtectedFunction(loaded, "on_debug_gui");
    return ScriptComponent(loaded, updateFn, onDebugGuiFn, absPath);
  }
};
