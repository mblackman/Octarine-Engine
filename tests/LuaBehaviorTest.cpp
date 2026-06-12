// Behavioral checks for the Lua binding surface — call bound component methods from Lua
// and verify the C++ component mutated. Layered on top of LuaApiSmokeTest, which only
// validates that the surface exists. Catches the "binding compiles, method does nothing"
// class of regression that drift checks miss.
//
// Headless Game (constructor allocates Registry/EventBus/Lua, opens no SDL window/renderer).

#include <sol/sol.hpp>
#include <string>

#include "Components/HealthComponent.h"
#include "Components/PositionComponent.h"
#include "ECS/Registry.h"
#include "Game/Game.h"
#include "General/Logger.h"
#include "Lua/Bindings/HealthComponentLuaBinding.h"
#include "Lua/Bindings/LuaBinding.h"
#include "Lua/Bindings/LuaComponentRegistry.h"
#include "Lua/Bindings/PositionComponentLuaBinding.h"
#include "Lua/Bindings/RegisterAllBindings.h"
#include "Lua/Modules/RegisterAllModules.h"
#include "Systems/ScriptSystem.h"
#include "TestHarness.h"

using octarine::test::Check;
using octarine::test::CheckEq;

namespace {
bool RunLua(sol::state& lua, const std::string& script) {
  const auto r = lua.safe_script(script, sol::script_pass_on_error);
  if (!r.valid()) {
    const sol::error err = r;
    std::cerr << "    lua error: " << err.what() << "\n";
    return false;
  }
  return true;
}
}  // namespace

int main() {
  // Logger::Init wires the lua_logger_ that Logger::LogLua / Logger::InfoLua dereference.
  // Without it, the bound Lua `log` / `log_i` functions crash on null logger.
  Logger::Init();

  Game game;  // SDL-free constructor.
  sol::state& lua = game.GetLua();
  lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::io, sol::lib::string, sol::lib::table);

  RegisterAllLuaBindings();
  ScriptSystem scriptSystem;
  scriptSystem.CreateLuaBindings(lua);  // primitives + every LuaBinding<T>::bindUsertype
  RegisterAllLuaModules(lua, game);     // installs `registry.has_/get_<key>` + free functions

  Registry* reg = game.GetRegistry();

  std::cout << "[health: damage / heal round-trip via Lua]\n";
  {
    const Entity e = reg->CreateEntity();
    reg->AddComponent<HealthComponent>(e, HealthComponent(100));
    lua["test_entity"] = e;

    Check(RunLua(lua, "registry.get_health(test_entity):damage(30)"), "Lua: damage(30) call succeeds");
    CheckEq(reg->GetComponent<HealthComponent>(e).currentHealth, 70, "health.damage(30) reduced currentHealth to 70");

    Check(RunLua(lua, "registry.get_health(test_entity):heal(10)"), "Lua: heal(10) call succeeds");
    CheckEq(reg->GetComponent<HealthComponent>(e).currentHealth, 80, "health.heal(10) bumped currentHealth to 80");

    // damage() floors at 0; heal() caps at maxHealth — invariants enforced by bound methods.
    Check(RunLua(lua, "registry.get_health(test_entity):damage(9999)"), "Lua: massive damage call succeeds");
    CheckEq(reg->GetComponent<HealthComponent>(e).currentHealth, 0, "damage() floors at 0");

    Check(RunLua(lua, "registry.get_health(test_entity):heal(9999)"), "Lua: massive heal call succeeds");
    CheckEq(reg->GetComponent<HealthComponent>(e).currentHealth, 100, "heal() caps at maxHealth");
  }

  std::cout << "[health: clamping property setter from Lua]\n";
  {
    const Entity e = reg->CreateEntity();
    reg->AddComponent<HealthComponent>(e, HealthComponent(50));
    lua["test_entity"] = e;

    // current_health is a clamping property; writing a value above max_health must clamp,
    // writing a negative value must clamp to 0.
    Check(RunLua(lua, "registry.get_health(test_entity).current_health = 999"), "Lua write current_health=999");
    CheckEq(reg->GetComponent<HealthComponent>(e).currentHealth, 50, "current_health setter clamps to maxHealth");

    Check(RunLua(lua, "registry.get_health(test_entity).current_health = -42"), "Lua write current_health=-42");
    CheckEq(reg->GetComponent<HealthComponent>(e).currentHealth, 0, "current_health setter clamps to 0");
  }

  std::cout << "[position: write field through bound usertype]\n";
  {
    const Entity e = reg->CreateEntity();
    reg->AddComponent<PositionComponent>(e, PositionComponent(glm::vec2(1.0F, 2.0F)));
    lua["test_entity"] = e;

    Check(RunLua(lua, "registry.get_position(test_entity).value = vec2.new(7, 9)"), "Lua write position.value");
    CheckEq(reg->GetComponent<PositionComponent>(e).value.x, 7.0F, "position.value.x written from Lua");
    CheckEq(reg->GetComponent<PositionComponent>(e).value.y, 9.0F, "position.value.y written from Lua");
  }

  std::cout << "[has_/get_ accessors]\n";
  {
    const Entity hasHealth = reg->CreateEntity();
    reg->AddComponent<HealthComponent>(hasHealth, HealthComponent(10));
    const Entity noHealth = reg->CreateEntity();

    lua["with_health"] = hasHealth;
    lua["without_health"] = noHealth;
    Check(RunLua(lua, "assert(registry.has_health(with_health))"), "registry.has_health true for owner");
    Check(RunLua(lua, "assert(not registry.has_health(without_health))"), "registry.has_health false for non-owner");
  }

  std::cout << "[blam: single entity and table of entities]\n";
  {
    const Entity single = reg->CreateEntity();
    lua["blam_single"] = single;
    Check(RunLua(lua, "blam(blam_single)"), "Lua: blam(entity) call succeeds");
    Check(reg->IsAlive(single), "blam(entity) defers destruction until end of frame");
    reg->Update(0.016F);
    Check(!reg->IsAlive(single), "blam(entity) destroyed the entity after Update");

    const Entity first = reg->CreateEntity();
    const Entity second = reg->CreateEntity();
    const Entity survivor = reg->CreateEntity();
    lua["blam_first"] = first;
    lua["blam_second"] = second;
    Check(RunLua(lua, "blam({ blam_first, blam_second })"), "Lua: blam(table) call succeeds");
    Check(reg->IsAlive(first) && reg->IsAlive(second), "blam(table) defers destruction until end of frame");
    reg->Update(0.016F);
    Check(!reg->IsAlive(first), "blam(table) destroyed the first entity after Update");
    Check(!reg->IsAlive(second), "blam(table) destroyed the second entity after Update");
    Check(reg->IsAlive(survivor), "blam(table) left unrelated entities alone");

    // Non-entity entries are skipped with an error log, not a Lua error.
    Check(RunLua(lua, "blam({ 42, 'nope' })"), "Lua: blam(table) tolerates non-entity entries");
  }

  std::cout << "[registry.set_parent / get_parent round-trip]\n";
  {
    const Entity parent = reg->CreateEntity();
    const Entity child = reg->CreateEntity();
    lua["hier_parent"] = parent;
    lua["hier_child"] = child;

    Check(RunLua(lua, "assert(registry.get_parent(hier_child) == nil)"), "get_parent is nil before set_parent");
    Check(RunLua(lua, "registry.set_parent(hier_child, hier_parent)"), "Lua: set_parent call succeeds");
    Check(reg->GetParent(child).has_value() && reg->GetParent(child)->id == parent.id,
          "set_parent wired child->parent in the registry");
    Check(RunLua(lua, "assert(registry.get_parent(hier_child):get_id() == hier_parent:get_id())"),
          "get_parent returns the parent handle");

    const Entity second = reg->CreateEntity();
    lua["hier_second"] = second;
    Check(RunLua(lua, "registry.set_parent(hier_child, hier_second)"), "Lua: reparent call succeeds");
    Check(reg->GetParent(child).has_value() && reg->GetParent(child)->id == second.id,
          "reparent moved the child to the new parent");
    Check(reg->GetChildren(parent).empty(), "reparent detached the child from the old parent");

    // Self-parenting and cycles are rejected with an error log, not a Lua error.
    Check(RunLua(lua, "registry.set_parent(hier_child, hier_child)"), "Lua: self-parent call tolerated");
    Check(reg->GetParent(child).has_value() && reg->GetParent(child)->id == second.id,
          "self-parent rejected, parent unchanged");
    Check(RunLua(lua, "registry.set_parent(hier_second, hier_child)"), "Lua: cycle call tolerated");
    Check(!reg->GetParent(second).has_value(), "cycle rejected, would-be grandparent unchanged");
  }

  std::cout << "[load_entity: returns the root entity handle]\n";
  {
    Check(RunLua(lua,
                 "spawned = load_entity({ name = 'HandleTest', components = { transform = { position = { x = 3, y = "
                 "4 } } } })"),
          "Lua: load_entity call succeeds");
    Check(RunLua(lua, "assert(spawned ~= nil)"), "load_entity returned a non-nil handle");
    Check(RunLua(lua, "assert(type(spawned:get_id()) == 'number')"), "handle exposes get_id()");
    Check(RunLua(lua, "assert(registry.has_position(spawned))"), "handle works with registry accessors");

    const Entity spawned = lua["spawned"].get<Entity>();
    CheckEq(reg->GetComponent<PositionComponent>(spawned).value.x, 3.0F,
            "handle resolves to the entity built from the table");
  }

  std::cout << "[fromLua: defaults applied when fields omitted]\n";
  {
    // HealthComponent has no default-constructor (deleted); fromLua must synthesize one
    // from an empty table using its 100-max default.
    const auto t = lua.script("return {}");
    const HealthComponent defaulted = LuaBinding<HealthComponent>::fromLua(t);
    CheckEq(defaulted.maxHealth, 100, "fromLua({}) gives default maxHealth=100");
    CheckEq(defaulted.currentHealth, 100, "fromLua({}) starts at full health");

    const auto t2 = lua.script("return { max_health = 25 }");
    const HealthComponent custom = LuaBinding<HealthComponent>::fromLua(t2);
    CheckEq(custom.maxHealth, 25, "fromLua reads max_health");
    CheckEq(custom.currentHealth, 25, "fromLua initializes current to max");
  }

  std::cout << "[module callable: log() runs]\n";
  {
    // No observable side effect to assert against without wiring a log sink — just confirm
    // the bound function exists and returns from a Lua call.
    Check(lua["log"].valid() && lua["log"].get_type() == sol::type::function, "log is bound as a function");
    Check(RunLua(lua, "log('LuaBehaviorTest: smoke from log()')"), "log('...') runs from Lua without error");
  }

  return octarine::test::ReportSummary("Lua behavior test");
}
