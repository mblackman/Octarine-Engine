#pragma once

// Single entry point that registers every component's LuaBinding<T> with LuaComponentRegistry.
// Call once during Game::Setup, before any Lua-binding consumers (ScriptSystem, LuaEntityLoader)
// observe the registry. Idempotent across reloads — clears prior registrations first.
void RegisterAllLuaBindings();
