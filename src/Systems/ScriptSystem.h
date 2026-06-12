#pragma once

#include <cstdint>
#include <glm/glm.hpp>
#include <sol/sol.hpp>

#include "Components/ScriptComponent.h"
#include "General/Color.h"
#include "General/Logger.h"
#include "Lua/Bindings/LuaComponentRegistry.h"

#ifdef OCTARINE_WITH_IMGUI
#include "sol_ImGui.h"
#endif

class ScriptSystem {
 public:
  ScriptSystem() = default;

  void CreateLuaBindings(sol::state& lua) {
    CreateLuaTypes(lua);

#ifdef OCTARINE_WITH_IMGUI
    sol_ImGui::Init(lua);
#endif
  }

  void operator()(const ContextFacade& context, ScriptComponent& script) const {
    if (script.updateFunction == sol::lua_nil) {
      return;
    }

    if (auto result = script.updateFunction(script.scriptTable, context.GetEntity(), context.GetDeltaTime());
        !result.valid()) {
      const sol::error err = result;
      Logger::ErrorLua(std::string(err.what()));
    }
  }

 private:
  void CreateLuaTypes(sol::state& lua) {
    // Primitives — not components; kept inline.
    lua.new_usertype<Entity>("entity", "get_id", &Entity::GetId);
    lua.new_usertype<glm::vec2>(
        "vec2", sol::constructors<glm::vec2(float, float), glm::vec2()>(), "x",
        sol::property([](const glm::vec2& v) { return v.x; }, [](glm::vec2& v, float f) { v.x = f; }), "y",
        sol::property([](const glm::vec2& v) { return v.y; }, [](glm::vec2& v, float f) { v.y = f; }));

    lua.new_usertype<octarine::Color>(
        "color",
        sol::constructors<octarine::Color(), octarine::Color(std::uint8_t, std::uint8_t, std::uint8_t, std::uint8_t)>(),
        "r",
        sol::property([](const octarine::Color& c) { return c.r; },
                      [](octarine::Color& c, std::uint8_t v) { c.r = v; }),
        "g",
        sol::property([](const octarine::Color& c) { return c.g; },
                      [](octarine::Color& c, std::uint8_t v) { c.g = v; }),
        "b",
        sol::property([](const octarine::Color& c) { return c.b; },
                      [](octarine::Color& c, std::uint8_t v) { c.b = v; }),
        "a",
        sol::property([](const octarine::Color& c) { return c.a; },
                      [](octarine::Color& c, std::uint8_t v) { c.a = v; }));

    // Component usertypes — driven by LuaComponentRegistry. Add a new component by
    // dropping a LuaBinding<T> header + one line in RegisterAllBindings.cpp.
    for (const auto& entry : LuaComponentRegistry::all()) {
      entry.bindUsertype(lua);
    }
  }
};
