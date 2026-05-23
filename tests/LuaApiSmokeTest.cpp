// Headless smoke test for the Lua binding surface.
//
// Constructs a real Game (its constructor is SDL-free — the real boot path also runs `Game game;`
// before SDL_Init) and replays Game::Setup's binding sequence without opening a window. Asserts
// that every registered component is reachable from Lua and that each free-function module and
// system surface actually installed. Catches the "wrote a module/binding but never wired it"
// class of bug at build/CI time instead of at runtime in the example project.
//
// Built only when OCTARINE_ENABLE_TESTS=ON. Exit code is the number of failed checks.

#include <sol/sol.hpp>

#include <iostream>
#include <string>
#include <vector>

#include "ECS/Registry.h"
#include "Game/Game.h"
#include "Lua/Bindings/InputSystemLuaBinding.h"
#include "Lua/Bindings/LuaComponentRegistry.h"
#include "Lua/Bindings/LuaSystemRegistry.h"
#include "Lua/Bindings/RegisterAllBindings.h"
#include "Lua/LuaApiManifest.h"
#include "Lua/Modules/RegisterAllModules.h"
#include "Systems/InputSystem.h"
#include "Systems/ScriptSystem.h"

namespace
{
    int g_failures = 0;

    void Check(const bool condition, const std::string& what)
    {
        if (condition)
        {
            std::cout << "  ok   " << what << "\n";
        }
        else
        {
            std::cerr << "  FAIL " << what << "\n";
            ++g_failures;
        }
    }

    bool IsFunction(sol::state& lua, const std::string& name)
    {
        const sol::object obj = lua[name];
        return obj.valid() && obj.get_type() == sol::type::function;
    }

    bool TableHasFunction(sol::state& lua, const std::string& table, const std::string& key)
    {
        const sol::object tableObj = lua[table];
        if (!tableObj.valid() || tableObj.get_type() != sol::type::table) return false;
        const sol::object fn = tableObj.as<sol::table>()[key];
        return fn.valid() && fn.get_type() == sol::type::function;
    }
} // namespace

int main()
{
    Game game; // SDL-free constructor — allocates Registry/EventBus/Renderer, opens no window.
    sol::state& lua = game.GetLua();
    lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::io, sol::lib::string, sol::lib::table);
    const auto preBinding = LuaApiManifest::SnapshotGlobals(lua);

    // Mirror Game::Setup's binding order: component types, then free-function modules, then
    // system surfaces.
    RegisterAllLuaBindings();

    ScriptSystem scriptSystem;
    scriptSystem.CreateLuaBindings(lua); // primitives + component usertypes

    RegisterAllLuaModules(lua, game); // free-function globals + the `registry` accessor table

    InputSystem inputSystem;
    LuaSystemRegistry::clear();
    LuaSystemRegistry::registerSystem(inputSystem);
    LuaSystemRegistry::bindAll(lua); // system surfaces (input.*)

    std::cout << "[components] usertypes + registry accessors\n";
    const auto& components = LuaComponentRegistry::all();
    Check(!components.empty(), "component registry is non-empty");
    for (const auto& entry : components)
    {
        const sol::object usertype = lua[entry.usertypeName];
        Check(usertype.valid(), "usertype '" + entry.usertypeName + "' bound");
        Check(TableHasFunction(lua, "registry", "has_" + entry.luaKey), "registry.has_" + entry.luaKey);
        Check(TableHasFunction(lua, "registry", "get_" + entry.luaKey), "registry.get_" + entry.luaKey);
    }

    std::cout << "[modules] one sentinel global per module\n";
    // If a module is dropped from RegisterAllModules.cpp, its sentinel disappears and this fails.
    const std::vector<std::pair<std::string, std::string>> moduleSentinels = {
        {"Log", "log"},     {"Io", "read_file_lines"}, {"Assets", "get_asset_path"}, {"Audio", "play_sound"},
        {"Entity", "blam"}, {"Scene", "load_scene"},   {"Game", "fire_projectile"},
    };
    for (const auto& [module, global] : moduleSentinels)
    {
        Check(IsFunction(lua, global), module + "Module installed (" + global + ")");
    }

    std::cout << "[primitives + systems]\n";
    Check(lua["vec2"].valid(), "vec2 usertype bound");
    Check(lua["entity"].valid(), "entity usertype bound");
    Check(lua["registry"].valid() && lua["registry"].get_type() == sol::type::table, "registry table present");
    Check(TableHasFunction(lua, "input", "is_key_down"), "input.is_key_down (InputSystem surface)");

    std::cout << "[manifest] generator runs\n";
    Check(LuaApiManifest::Write(lua, preBinding, "lua_api.smoke.lua"), "manifest written");

    if (g_failures == 0)
    {
        std::cout << "\nLua API smoke test: PASS\n";
    }
    else
    {
        std::cerr << "\nLua API smoke test: " << g_failures << " FAILED\n";
    }
    return g_failures;
}
