// Headless smoke test for the Lua binding surface.
//
// Constructs a real Game (its constructor is SDL-free — the real boot path also runs `Game game;`
// before SDL_Init) and replays Game::Setup's binding sequence without opening a window. Asserts
// that every registered component is reachable from Lua and that each free-function module and
// system surface actually installed. Catches the "wrote a module/binding but never wired it"
// class of bug at build/CI time instead of at runtime in the example project.
//
// Built only when OCTARINE_ENABLE_TESTS=ON. Exit code is the number of failed checks.

#include <filesystem>
#include <fstream>
#include <iostream>
#include <set>
#include <sol/sol.hpp>
#include <string>
#include <vector>

#include "Components/ScriptComponent.h"
#include "ECS/Registry.h"
#include "Game/Game.h"
#include "Game/GameConfig.h"
#include "General/Logger.h"
#include "Lua/HotReload/ScriptHotReload.h"
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

namespace {
int g_failures = 0;

void Check(const bool condition, const std::string& what) {
  if (condition) {
    std::cout << "  ok   " << what << "\n";
  } else {
    std::cerr << "  FAIL " << what << "\n";
    ++g_failures;
  }
}

bool IsFunction(sol::state& lua, const std::string& name) {
  const sol::object obj = lua[name];
  return obj.valid() && obj.get_type() == sol::type::function;
}

bool TableHasFunction(sol::state& lua, const std::string& table, const std::string& key) {
  const sol::object tableObj = lua[table];
  if (!tableObj.valid() || tableObj.get_type() != sol::type::table) return false;
  const sol::object fn = tableObj.as<sol::table>()[key];
  return fn.valid() && fn.get_type() == sol::type::function;
}
}  // namespace

int main() {
  // Logger::Init brings up the dual main/lua spdlog sinks. ScriptHotReload calls
  // Logger::ErrorLua on failed reloads; without Init the lua sink shared_ptr is null and the
  // call AVs. Real binary calls this from Main; tests must do it explicitly.
  Logger::Init();

  Game game;  // SDL-free constructor — allocates Registry/EventBus/Renderer, opens no window.
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
  scriptSystem.CreateLuaBindings(lua);  // primitives + component usertypes

#ifdef OCTARINE_WITH_EDITOR
  // ProjectModule installs only in an editor session (its install() checks IsEditorMode).
  // Force one here so the module registers its globals and modules.json captures them.
  game.GetRegistry()->Set<GameConfig>(GameConfig());
  game.GetRegistry()->Get<GameConfig>().SetIsEditorMode(true);
#endif

  // Install modules one at a time, snapshotting globals around each call so LuaModuleRegistry
  // ends up with an exact "what globals did each module install" record — fed to modules.json.
  LuaModuleRegistry::clear();
  for (const auto& m : kLuaModules) {
    const auto before = LuaApiManifest::SnapshotGlobals(lua);
    m.install(lua, game);
    const auto after = LuaApiManifest::SnapshotGlobals(lua);
    std::vector<std::string> added;
    for (const auto& name : after) {
      if (before.find(name) == before.end()) added.push_back(name);
    }
    LuaModuleRegistry::registerModule(m.name, std::move(added));
  }

  InputSystem inputSystem;
  LuaSystemRegistry::clear();
  LuaSystemRegistry::registerSystem(inputSystem);
  LuaSystemRegistry::bindAll(lua);  // system surfaces (input.*)

  std::cout << "[components] usertypes + registry accessors\n";
  const auto& components = LuaComponentRegistry::all();
  Check(!components.empty(), "component registry is non-empty");
  for (const auto& entry : components) {
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
    for (const auto& entry : inspectors) {
      Check(names.insert(entry.displayName).second, "inspector display name unique: " + entry.displayName);
    }

    // Every addable inspector's makeDefault() must yield a component that has() can find —
    // catches a broken default ctor or a mis-wired registerComponent<T>().
    auto* reg = game.GetRegistry();
    for (const auto& entry : inspectors) {
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
  for (const auto& m : LuaModuleRegistry::all()) {
    Check(!m.globals.empty(), m.name + "Module installed at least one global");
  }

  std::cout << "[primitives + systems]\n";
  Check(lua["vec2"].valid(), "vec2 usertype bound");
  Check(lua["entity"].valid(), "entity usertype bound");
  Check(lua["registry"].valid() && lua["registry"].get_type() == sol::type::table, "registry table present");
  Check(TableHasFunction(lua, "input", "is_key_down"), "input.is_key_down (InputSystem surface)");

  std::cout << "[file write surface] storage.* (all builds) + project.* (editor sessions)\n";
  for (const auto* fn : {"write", "read", "exists", "remove", "list"}) {
    Check(TableHasFunction(lua, "storage", fn), std::string("storage.") + fn);
  }
#ifdef OCTARINE_WITH_EDITOR
  Check(TableHasFunction(lua, "project", "path"), "project.path (editor session)");
  Check(TableHasFunction(lua, "project", "write_file"), "project.write_file (editor session)");
#endif

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

#ifndef OCTARINE_SHIPPED
  std::cout << "[hot-reload] ScriptHotReload swap + state preservation + error containment\n";
  {
    auto* reg = game.GetRegistry();
    reg->Set<ScriptHotReload>(ScriptHotReload());
    auto& hotReload = reg->Get<ScriptHotReload>();

    const auto tempDir = std::filesystem::temp_directory_path() / "octarine-hotreload-test";
    std::error_code ec;
    std::filesystem::create_directories(tempDir, ec);
    const auto scriptPath = (tempDir / "fixture.lua").string();

    const auto WriteScript = [&](const std::string& body) {
      std::ofstream f(scriptPath, std::ios::trunc);
      f << body;
      f.close();
    };

    // v1: on_update writes 'kind = "v1"' into self.data.
    WriteScript(
        "return {\n"
        "  data = { kind = 'initial', persisted = 42 },\n"
        "  on_update = function(self, e, dt) self.data.kind = 'v1' end,\n"
        "}\n");

    // Load v1 manually via dofile (mirrors what FromSource does in the binding).
    sol::protected_function_result loadV1 = lua["dofile"](scriptPath);
    Check(loadV1.valid(), "fixture v1 loads");
    sol::table v1Table = loadV1;
    const sol::protected_function v1Update = v1Table["on_update"];

    // Author-side mutation before reload: bump a value that must survive the swap.
    v1Table["data"]["persisted"] = 99;

    ScriptComponent sc(v1Table, v1Update, sol::protected_function(sol::lua_nil), scriptPath);
    const Entity ent = reg->CreateEntity();
    reg->AddComponent(ent, std::move(sc));

    // Drive v1 once so 'kind' is set to "v1".
    {
      auto& live = reg->GetComponent<ScriptComponent>(ent);
      live.updateFunction(live.scriptTable, ent, 0.016F);
      sol::optional<std::string> kindOpt = live.scriptTable["data"]["kind"];
      Check(kindOpt.value_or("<missing>") == "v1", "v1 on_update fires (kind == 'v1')");
    }

    // v2: same file, different behavior. Save *should* be picked up by ForceReloadAll.
    WriteScript(
        "return {\n"
        "  data = { kind = 'shadow', persisted = 0 },\n"
        "  on_update = function(self, e, dt) self.data.kind = 'v2' end,\n"
        "}\n");
    hotReload.ForceReloadAll(*reg, lua);
    {
      auto& live = reg->GetComponent<ScriptComponent>(ent);
      live.updateFunction(live.scriptTable, ent, 0.016F);
      sol::optional<sol::table> dataOpt = live.scriptTable["data"];
      sol::optional<std::string> kindOpt = dataOpt ? dataOpt->get<sol::optional<std::string>>("kind") : sol::nullopt;
      Check(kindOpt.value_or("<missing>") == "v2", "v2 on_update fires after reload (kind == 'v2')");
      sol::optional<int> persistedOpt = dataOpt ? dataOpt->get<sol::optional<int>>("persisted") : sol::nullopt;
      Check(persistedOpt.value_or(-1) == 99, "scriptTable.data preserved across reload (persisted == 99)");
      Check(hotReload.LastReload().error.empty(), "successful reload leaves no last-error");
    }

    // Syntax error: reload must log + leave prior callbacks intact.
    WriteScript("return { on_update = function(self, e, dt) self.data.kind = 'v3' end,\n");  // missing closing }
    hotReload.ForceReloadAll(*reg, lua);
    {
      auto& live = reg->GetComponent<ScriptComponent>(ent);
      live.updateFunction(live.scriptTable, ent, 0.016F);
      sol::optional<std::string> kindOpt = live.scriptTable["data"]["kind"];
      Check(kindOpt.value_or("<missing>") == "v2", "syntax-error reload preserves prior callback (kind still 'v2')");
      Check(!hotReload.LastReload().error.empty(), "failed reload records error");
    }

    std::filesystem::remove_all(tempDir, ec);
  }
#endif

  if (g_failures == 0) {
    std::cout << "\nLua API smoke test: PASS\n";
  } else {
    std::cerr << "\nLua API smoke test: " << g_failures << " FAILED\n";
  }
  return g_failures;
}
