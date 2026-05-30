#pragma once

#include <sol/sol.hpp>

#include <string>
#include <unordered_set>

// Generates an EmmyLua-annotated stub of the engine's scripting surface by introspecting a live
// sol::state after all bindings are installed. Drives editor autocomplete and serves as a living
// reference that cannot drift from the C++ that produced it — there is no hand-maintained name
// list. Add a global/usertype anywhere and it shows up here automatically.
namespace LuaApiManifest
{
    // Snapshot the names of the current globals. Call right after lua.open_libraries(...) and
    // before binding the engine surface; the diff against post-binding globals isolates exactly
    // what the engine added (excluding the Lua standard library).
    std::unordered_set<std::string> SnapshotGlobals(sol::state& lua);

    // Write EmmyLua stubs for every global added since `before`. Returns false if the file could
    // not be opened.
    bool Write(sol::state& lua, const std::unordered_set<std::string>& before, const std::string& outPath);

    // Convenience: read the OCTARINE_DUMP_LUA_API env var and, if set, write the manifest.
    // Value is the output path; the literal "1" maps to "lua_api.lua" in the working directory.
    // No-op (returns false) when the var is unset. Called from Game::Setup.
    bool MaybeDumpFromEnv(sol::state& lua, const std::unordered_set<std::string>& before);

    // Emit a machine-readable JSON index of every component registered with LuaComponentRegistry:
    // lua_key, usertype name, and accessor function names exposed on the `registry` table. Agents
    // grep this instead of crawling src/Lua/Bindings/. Returns false if the file cannot be opened.
    bool WriteComponentsJson(const std::string& outPath);

    // Emit a machine-readable JSON index of every Lua free-function module registered with
    // LuaModuleRegistry: module name + the globals it installed into sol::state. Returns false
    // if the file cannot be opened.
    bool WriteModulesJson(const std::string& outPath);
} // namespace LuaApiManifest
