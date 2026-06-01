#pragma once

#include <sol/sol.hpp>
#include <vector>

class LuaBindingContext;

// Descriptor for a single Lua free-function module. The install function pointer matches
// LuaModuleBinding<M>::install (sol::state&, LuaBindingContext&); the name is used by the modules.json index.
struct LuaModuleDescriptor {
  const char* name;
  void (*install)(sol::state&, LuaBindingContext&);
};

// Authoritative install order. RegisterAllLuaModules iterates this; OctarineLuaApiTest iterates
// the same list with per-module snapshots to discover which globals each module installs.
extern const std::vector<LuaModuleDescriptor> kLuaModules;

void RegisterAllLuaModules(sol::state& lua, LuaBindingContext& ctx);
