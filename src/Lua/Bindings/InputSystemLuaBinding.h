#pragma once

#include <sol/sol.hpp>

#include "Lua/Bindings/LuaSystemRegistry.h"
#include "Systems/InputSystem.h"

template <>
struct LuaSystemBinding<InputSystem>
{
    static void bind(sol::state& lua, InputSystem& system) { system.CreateLuaBindings(lua); }
};
