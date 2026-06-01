#pragma once

#include <sol/sol.hpp>

#include "Lua/Bindings/LuaSystemRegistry.h"

class InputSystem;

// LuaSystemBinding<InputSystem>::bind installs the `input.*` table — every is_/on_/bind/etc.
// global the scenes reach. Definition lives in InputSystemLuaBinding.cpp so InputSystem.h
// doesn't have to host the sol-laden install body inline.
template <>
struct LuaSystemBinding<InputSystem> {
  static void bind(sol::state& lua, InputSystem& system);
};
