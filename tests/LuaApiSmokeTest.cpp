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
#include <set>
#include <string>
#include <vector>

#include "ECS/Registry.h"
#include "Game/Game.h"
#ifdef OCTARINE_WITH_EDITOR
#include "Editor/Inspectors/ComponentInspectorRegistry.h"
#include "Editor/Inspectors/RegisterAllInspectors.h"
#endif
#include "Lua/Bindings/InputSystemLuaBinding.h"
#include "Lua/Bindings/LuaComponentRegistry.h"
#include "Lua/Bindings/LuaSystemRegistry.h"
#include "Lua/Bindings/RegisterAllBindings.h"
#include "Lua/LuaApiManifest.h"
#include "Lua/Modules/LuaModuleRegistry.h"
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
#ifdef OCTARINE_WITH_EDITOR
    RegisterAllComponentInspectors();
#endif

    ScriptSystem scriptSystem;
    scriptSystem.CreateLuaBindings(lua); // primitives + component usertypes

    // Install modules one at a time, snapshotting globals around each call so LuaModuleRegistry
    // ends up with an exact "what globals did each module install" record — fed to modules.json.
    LuaModuleRegistry::clear();
    for (const auto& m : kLuaModules)
    {
        const auto before = LuaApiManifest::SnapshotGlobals(lua);
        m.install(lua, game);
        const auto after = LuaApiManifest::SnapshotGlobals(lua);
        std::vector<std::string> added;
        for (const auto& name : after)
        {
            if (before.find(name) == before.end()) added.push_back(name);
        }
        LuaModuleRegistry::registerModule(m.name, std::move(added));
    }

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

#ifdef OCTARINE_WITH_EDITOR
    std::cout << "[inspectors] component inspector registry\n";
    {
        const auto& inspectors = ComponentInspectorRegistry::all();
        Check(!inspectors.empty(), "inspector registry is non-empty");

        std::set<std::string> names;
        for (const auto& entry : inspectors)
        {
            Check(names.insert(entry.displayName).second, "inspector display name unique: " + entry.displayName);
        }

        // Every addable inspector's makeDefault() must yield a component that has() can find —
        // catches a broken default ctor or a mis-wired registerComponent<T>().
        auto* reg = game.GetRegistry();
        for (const auto& entry : inspectors)
        {
            if (!entry.addDefault) continue;
            const Entity ent = reg->CreateEntity();
            entry.addDefault(reg, ent);
            Check(entry.has(reg, ent), "addDefault produces has(): " + entry.displayName);
        }
    }
#endif

    std::cout << "[modules] each module installs at least one global\n";
    // If a module's install body becomes empty (or was removed), its captured global list is empty
    // and this fails. Catches "dropped from RegisterAllModules.cpp" without naming sentinels by hand.
    Check(!LuaModuleRegistry::all().empty(), "module registry is non-empty");
    for (const auto& m : LuaModuleRegistry::all())
    {
        Check(!m.globals.empty(), m.name + "Module installed at least one global");
    }

    std::cout << "[primitives + systems]\n";
    Check(lua["vec2"].valid(), "vec2 usertype bound");
    Check(lua["entity"].valid(), "entity usertype bound");
    Check(lua["registry"].valid() && lua["registry"].get_type() == sol::type::table, "registry table present");
    Check(TableHasFunction(lua, "input", "is_key_down"), "input.is_key_down (InputSystem surface)");

    std::cout << "[manifest] generator runs\n";
#ifdef LUA_API_SMOKE_OUTPUT
    const std::string smokeOutPath = LUA_API_SMOKE_OUTPUT;
#else
    const std::string smokeOutPath = "lua_api.smoke.lua";
#endif
    Check(LuaApiManifest::Write(lua, preBinding, smokeOutPath), "manifest written");

#ifdef LUA_API_COMPONENTS_OUTPUT
    Check(LuaApiManifest::WriteComponentsJson(LUA_API_COMPONENTS_OUTPUT), "components.json written");
#endif
#ifdef LUA_API_MODULES_OUTPUT
    Check(LuaApiManifest::WriteModulesJson(LUA_API_MODULES_OUTPUT), "modules.json written");
#endif

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
