#pragma once

#include <cstdint>
#include <glm/glm.hpp>
#include <sol/sol.hpp>

#include "../Components/ScriptComponent.h"
#include "../General/Color.h"
#include "../General/Logger.h"
#include "Lua/Bindings/LuaComponentRegistry.h"

#ifdef OCTARINE_WITH_IMGUI
#include "sol_ImGui.h"
#endif

class ScriptSystem
{
public:
    ScriptSystem() = default;

    void CreateLuaBindings(sol::state& lua)
    {
        CreateLuaTypes(lua);

#ifdef OCTARINE_WITH_IMGUI
        sol_ImGui::Init(lua);
#endif
    }

    void operator()(const ContextFacade& context, ScriptComponent& script) const
    {
        if (script.updateFunction == sol::lua_nil)
        {
            return;
        }

        if (auto result = script.updateFunction(script.scriptTable, context.GetEntity(), context.GetDeltaTime());
            !result.valid())
        {
            const sol::error err = result;
            Logger::ErrorLua(std::string(err.what()));
        }
    }

private:
    void CreateLuaTypes(sol::state& lua)
    {
        // Primitives — not components; kept inline.
        lua.new_usertype<Entity>("entity", "get_id", &Entity::GetId);
        lua.new_usertype<glm::vec2>("vec2", sol::constructors<glm::vec2(float, float), glm::vec2()>(), "x",
                                    &glm::vec2::x,
                                    "y", &glm::vec2::y);

        lua.new_usertype<octarine::Color>(
            "color",
            sol::constructors<octarine::Color(), octarine::Color(std::uint8_t, std::uint8_t, std::uint8_t, std::uint8_t)>(),
            "r", &octarine::Color::r, "g", &octarine::Color::g, "b", &octarine::Color::b, "a", &octarine::Color::a);

        // Component usertypes — driven by LuaComponentRegistry. Add a new component by
        // dropping a LuaBinding<T> header + one line in RegisterAllBindings.cpp.
        for (const auto& entry : LuaComponentRegistry::all())
        {
            entry.bindUsertype(lua);
        }
    }
};
