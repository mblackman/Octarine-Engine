#pragma once

#include <sol/sol.hpp>

class Game;

// Specialize for each module to expose a group of free-function globals to Lua.
// Each specialization MUST provide:
//   static void install(sol::state& lua, Game& game);
//
// Modules install during Game::Setup (after CreateLuaBindings types, before
// LuaSystemRegistry::bindAll). Add a new module by creating a header with a tag
// struct + LuaModuleBinding specialization, defining install() in a sibling .cpp,
// and adding one line to RegisterAllModules.cpp.
template <typename M>
struct LuaModuleBinding;
