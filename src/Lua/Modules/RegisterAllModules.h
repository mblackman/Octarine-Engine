#pragma once

#include <sol/sol.hpp>

class Game;

void RegisterAllLuaModules(sol::state& lua, Game& game);
