#pragma once

#include <SDL3/SDL.h>

#include <glm/glm.hpp>
#include <sol/sol.hpp>
#include <string>

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

    inline SDL_Color SafeGetColor(const sol::table& parentTable, const std::string& key, Uint8 defaultR = 0,
                                  Uint8 defaultG = 0, Uint8 defaultB = 0, Uint8 defaultA = 0)
    {
        SDL_Color result = {defaultR, defaultG, defaultB, defaultA};
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
template <typename T>
struct LuaBinding;
