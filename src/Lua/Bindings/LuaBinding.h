#pragma once

#include <cstdint>
#include <glm/glm.hpp>
#include <sol/sol.hpp>
#include <string>

#include "General/Color.h"

namespace LuaComponentHelpers
{
    inline glm::vec2 SafeGetVec2(const sol::table& parentTable, const std::string& key, float defaultX = 0.0f,
                                 float defaultY = 0.0f)
    {
        glm::vec2 result(defaultX, defaultY);
        if (sol::optional<sol::table> vecTableOpt = parentTable[key]; vecTableOpt && vecTableOpt.value().valid())
        {
            sol::table& vecTable = vecTableOpt.value();
            result.x = vecTable["x"].get_or(defaultX);
            result.y = vecTable["y"].get_or(defaultY);
        }
        return result;
    }

    inline octarine::Color SafeGetColor(const sol::table& parentTable, const std::string& key,
                                        std::uint8_t defaultR = 0, std::uint8_t defaultG = 0,
                                        std::uint8_t defaultB = 0, std::uint8_t defaultA = 0)
    {
        octarine::Color result{defaultR, defaultG, defaultB, defaultA};
        if (sol::optional<sol::table> vecTableOpt = parentTable[key]; vecTableOpt && vecTableOpt.value().valid())
        {
            sol::table& vecTable = vecTableOpt.value();
            result.r = vecTable["r"].get_or(defaultR);
            result.g = vecTable["g"].get_or(defaultG);
            result.b = vecTable["b"].get_or(defaultB);
            result.a = vecTable["a"].get_or(defaultA);
        }
        return result;
    }

    template <typename T>
    T SafeGetOptionalValue(const sol::table& dataTable, const std::string& key, T defaultValue)
    {
        return dataTable[key].get_or(defaultValue);
    }

    inline sol::protected_function SafeGetProtectedFunction(const sol::table& dataTable, const std::string& key)
    {
        return dataTable[key].get_or(sol::protected_function(sol::lua_nil));
    }

    inline sol::optional<sol::table> SafeGetOptionalTable(const sol::table& dataTable, const std::string& key)
    {
        if (dataTable[key].is<sol::table>())
        {
            return dataTable[key].get<sol::table>();
        }
        return sol::nullopt;
    }
} // namespace LuaComponentHelpers

// Specialize for each component to expose it to Lua. Each specialization MUST provide:
//   static constexpr const char* kLuaKey;          // entity-table key, e.g. "health"
//   static constexpr const char* kUsertypeName;    // sol2 usertype name, e.g. "health_component"
//   static T fromLua(const sol::object& data);     // sol::object (not table) so bare-string/int edge cases fit
//   static void bindUsertype(sol::state& lua);     // call lua.new_usertype<T>(kUsertypeName, ...);
//
// bindUsertype may include both fields and member functions in the new_usertype call,
// e.g.  "damage", &HealthComponent::damage  alongside  "max_health", &T::maxHealth.
//
// Component-method hard rule: a component method may read/write the component's own fields
// ONLY. No Registry&, no EventBus*, no cross-entity reads/writes, no archetype mutation, no
// allocation, no event emission. Anything crossing those lines belongs in a system. The
// moment a method needs side effects beyond its own fields, move it out.
//
// For derived read-only state (isDead, fraction), bind with sol::property so scripts get
// field-style access:  "is_dead", sol::property(&T::isDead).
// For fields with invariants (currentHealth clamped to [0, maxHealth]), bind with
// sol::property + a clamping setter so Lua can't bypass via direct field assignment.
template <typename T>
struct LuaBinding;
