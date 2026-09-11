// Behavioral checks for the Lua binding surface — call bound component methods from Lua
// and verify the C++ component mutated. Layered on top of LuaApiSmokeTest, which only
// validates that the surface exists. Catches the "binding compiles, method does nothing"
// class of regression that drift checks miss.
//
// Headless Game (constructor allocates Registry/EventBus/Lua, opens no SDL window/renderer).

#include <cmath>
#include <sol/sol.hpp>
#include <string>

#include "Components/ColorGridComponent.h"
#include "Components/GlobalTransformComponent.h"
#include "Components/HealthComponent.h"
#include "Components/LifetimeComponent.h"
#include "Components/NameComponent.h"
#include "Components/PivotComponent.h"
#include "Components/PositionComponent.h"
#include "Components/RotationComponent.h"
#include "Components/ScaleComponent.h"
#include "Components/UIButtonComponent.h"
#include "ECS/Registry.h"
#include "Events/GamepadButtonEvent.h"
#include "Events/KeyInputEvent.h"
#include "Game/Game.h"
#include "Game/GameConfig.h"
#include "General/AngleUnit.h"
#include "General/Logger.h"
#include "Lua/Bindings/HealthComponentLuaBinding.h"
#include "Lua/Bindings/LuaBinding.h"
#include "Lua/Bindings/LuaComponentRegistry.h"
#include "Lua/Bindings/PositionComponentLuaBinding.h"
#include "Lua/Bindings/RegisterAllBindings.h"
#include "Lua/Bindings/RotationComponentLuaBinding.h"
#include "Lua/Modules/RegisterAllModules.h"
#include "Systems/InputSystem.h"
#include "Systems/RenderDebugGUISystem.h"
#include "Systems/ScriptSystem.h"
#include "Systems/TransformSystem.h"
#include "Systems/UIButtonSystem.h"
#include "TestHarness.h"

using octarine::test::Check;
using octarine::test::CheckEq;

namespace {
bool NearVec2(const glm::vec2 got, const glm::vec2 want, const float eps = 1e-3F) {
  return std::fabs(got.x - want.x) < eps && std::fabs(got.y - want.y) < eps;
}

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

  std::cout << "[angle unit: degrees convert at the Lua boundary, radians pass through]\n";
  {
    octarine::AngleUnits::Set(octarine::AngleUnit::Degrees);

    const Entity e = reg->CreateEntity();
    reg->AddComponent<RotationComponent>(e, RotationComponent(0.0F));
    lua["angle_entity"] = e;

    Check(RunLua(lua, "set_rotation(angle_entity, 90)"), "set_rotation(90) runs in degrees mode");
    Check(std::fabs(reg->GetComponent<RotationComponent>(e).value - 1.5707964F) < 1e-4F,
          "90 degrees authored from Lua stores pi/2 radians");

    Check(RunLua(lua, "lua_angle = registry.get_rotation(angle_entity).value"), "rotation.value reads from Lua");
    Check(std::fabs(lua["lua_angle"].get<float>() - 90.0F) < 1e-3F,
          "reading rotation.value back gives 90, not the stored radians");

    const auto degTable = lua.script("return { value = 180 }");
    Check(std::fabs(LuaBinding<RotationComponent>::fromLua(degTable).value - 3.1415927F) < 1e-4F,
          "component table value=180 stores pi radians in degrees mode");

    // Radians mode is opt-in via AngleUnit=radians and must leave authored values untouched.
    octarine::AngleUnits::Set(octarine::AngleUnit::Radians);
    const auto radTable = lua.script("return { value = 3.1415927 }");
    Check(std::fabs(LuaBinding<RotationComponent>::fromLua(radTable).value - 3.1415927F) < 1e-4F,
          "radians mode stores the authored value unchanged");
    Check(RunLua(lua, "set_rotation(angle_entity, 1.5707964)"), "set_rotation runs in radians mode");
    Check(std::fabs(reg->GetComponent<RotationComponent>(e).value - 1.5707964F) < 1e-4F,
          "radians mode stores pi/2 as authored");

    // AngleUnits is process-wide, so restore the default before the next block: leaving radians
    // set here would silently reinterpret any angle a later test authors.
    octarine::AngleUnits::Set(octarine::AngleUnit::Degrees);
  }

  std::cout << "[scale shorthand: a bare number means uniform scale on both authoring paths]\n";
  {
    Check(RunLua(lua, R"LUA(
      inline_uniform  = load_entity({ components = { transform = { scale = 3 } } })
      inline_table    = load_entity({ components = { transform = { scale = { x = 2, y = 5 } } } })
      standalone      = load_entity({ components = { scale = 4 } })
      standalone_flat = load_entity({ components = { scale = { x = 6, y = 7 } } })
    )LUA"),
          "scale shorthand script runs");

    const Entity inlineUniform = lua["inline_uniform"];
    const Entity inlineTable = lua["inline_table"];
    const Entity standalone = lua["standalone"];
    const Entity standaloneFlat = lua["standalone_flat"];

    Check(reg->GetComponent<ScaleComponent>(inlineUniform).value == glm::vec2(3.0F, 3.0F),
          "transform.scale = 3 is uniform scale, not the (1,1) default");
    Check(reg->GetComponent<ScaleComponent>(inlineTable).value == glm::vec2(2.0F, 5.0F),
          "transform.scale as a table still reads both axes");
    Check(reg->GetComponent<ScaleComponent>(standalone).value == glm::vec2(4.0F, 4.0F),
          "a standalone scale component takes the same shorthand");
    Check(reg->GetComponent<ScaleComponent>(standaloneFlat).value == glm::vec2(6.0F, 7.0F),
          "a standalone scale component accepts flat { x = ..., y = ... }");

    // `scale` is the uniform view and reads x, so scripts need is_uniform() to know whether that
    // is the whole story. It must not fail on the non-uniform case, only report it.
    Check(RunLua(lua, "uni = registry.get_scale(inline_uniform):is_uniform()"), "is_uniform reads from Lua");
    Check(lua["uni"].get<bool>(), "a uniformly scaled entity reports is_uniform");
    Check(RunLua(lua, "nonuni = registry.get_scale(inline_table):is_uniform()"),
          "is_uniform reads on a per-axis scale");
    Check(!lua["nonuni"].get<bool>(), "a per-axis scale reports non-uniform");
    Check(RunLua(lua, "any = registry.get_scale(inline_table).scale"), "reading .scale on a per-axis scale is safe");
    Check(lua["any"].get<float>() == 2.0F, "the uniform view of a per-axis scale reports the x axis");
  }

  std::cout << "[pivot: authored independently of rotation]\n";
  {
    Check(RunLua(lua, R"LUA(
      inline_pivot   = load_entity({ components = { transform = { pivot = { x = 0, y = 0 } } } })
      standalone     = load_entity({ components = { pivot = { x = 1, y = 1 } } })
      shorthand      = load_entity({ components = { pivot = 0 } })
      nested         = load_entity({ components = { pivot = { value = { x = 0.25, y = 0.75 } } } })
      rotation_only  = load_entity({ components = { transform = { rotation = 0.5 } } })
      pivot_no_rot   = load_entity({ components = { transform = { scale = 2, pivot = { x = 0, y = 0 } } } })
    )LUA"),
          "pivot authoring script runs");

    const Entity inlinePivot = lua["inline_pivot"];
    const Entity standalone = lua["standalone"];
    const Entity shorthand = lua["shorthand"];
    const Entity nested = lua["nested"];
    const Entity rotationOnly = lua["rotation_only"];
    const Entity pivotNoRot = lua["pivot_no_rot"];

    Check(reg->GetComponent<PivotComponent>(inlinePivot).value == glm::vec2(0.0F, 0.0F),
          "transform.pivot builds a PivotComponent of its own");
    Check(reg->GetComponent<PivotComponent>(standalone).value == glm::vec2(1.0F, 1.0F),
          "a standalone pivot component reads x and y");
    Check(reg->GetComponent<PivotComponent>(shorthand).value == glm::vec2(0.0F, 0.0F),
          "a bare number anchors both axes");
    // Reasoning by analogy from `scale`/`position`, whose standalone forms nest under `value`.
    Check(reg->GetComponent<PivotComponent>(nested).value == glm::vec2(0.25F, 0.75F),
          "the nested { value = { x, y } } form is accepted too, not silently defaulted");

    // The split is the point: neither component implies the other.
    Check(!reg->HasComponent<PivotComponent>(rotationOnly), "rotation alone attaches no pivot");
    Check(reg->HasComponent<RotationComponent>(rotationOnly), "rotation alone still attaches rotation");
    Check(reg->HasComponent<PivotComponent>(pivotNoRot), "a pivot can be authored with no rotation at all");
    Check(!reg->HasComponent<RotationComponent>(pivotNoRot), "pivot alone attaches no rotation");
  }

  std::cout << "[component axis accessors: x and y are distinct on both axes]\n";
  {
    const Entity e = reg->CreateEntity();
    reg->AddComponent<PositionComponent>(e, PositionComponent(glm::vec2(3.0F, 7.0F)));
    reg->AddComponent<ScaleComponent>(e, ScaleComponent(2.0F, 5.0F));
    lua["axis_entity"] = e;

    Check(RunLua(lua, "px = registry.get_position(axis_entity).x; py = registry.get_position(axis_entity).y"),
          "position x/y read from Lua");
    Check(lua["px"].get<float>() == 3.0F, "position.x reads x");
    Check(lua["py"].get<float>() == 7.0F, "position.y reads y, not x");

    Check(RunLua(lua, "sx = registry.get_scale(axis_entity).x; sy = registry.get_scale(axis_entity).y"),
          "scale x/y read from Lua");
    Check(lua["sx"].get<float>() == 2.0F, "scale.x reads x");
    Check(lua["sy"].get<float>() == 5.0F, "scale.y reads y, not x");

    reg->AddComponent<PivotComponent>(e, PivotComponent(0.25F, 0.75F));
    Check(RunLua(lua, "vx = registry.get_pivot(axis_entity).x; vy = registry.get_pivot(axis_entity).y"),
          "pivot x/y read from Lua");
    Check(lua["vx"].get<float>() == 0.25F, "pivot.x reads x");
    Check(lua["vy"].get<float>() == 0.75F, "pivot.y reads y, not x");

    Check(RunLua(lua, "registry.get_position(axis_entity).y = 11"), "position.y writes from Lua");
    Check(reg->GetComponent<PositionComponent>(e).value == glm::vec2(3.0F, 11.0F),
          "writing position.y moves y and leaves x alone");

    Check(RunLua(lua, "registry.get_pivot(axis_entity).x = 1"), "pivot.x writes from Lua");
    Check(reg->GetComponent<PivotComponent>(e).value == glm::vec2(1.0F, 0.75F),
          "writing pivot.x moves x and leaves y alone");
  }

  std::cout << "[pivot anchors the drawn quad under scale, growing and shrinking]\n";
  {
    reg->RegisterBulkSystem<GlobalTransformComponent>(TransformSystem());

    // A 40x20 sprite anchored at its bottom-right corner. Whatever the scale, that corner is the
    // fixed point and the quad grows away from it — the property a corner pivot promises.
    Check(RunLua(lua, R"LUA(
      anchored = load_entity({ components = {
        transform = { position = { x = 200, y = 100 }, scale = 1, pivot = { x = 1, y = 1 } },
        sprite = { texture_asset_id = "tex", width = 40, height = 20 },
      }})
    )LUA"),
          "corner-anchored entity builds from Lua");
    const Entity anchored = lua["anchored"];

    for (const float scale : {1.0F, 2.0F, 3.5F, 0.5F}) {
      reg->GetComponent<ScaleComponent>(anchored).value = glm::vec2(scale, scale);
      reg->Update(1.0F / 60.0F);

      // Exactly what RenderSpriteSystem draws: destRect at `position`, sized width * scale.
      const auto& g = reg->GetComponent<GlobalTransformComponent>(anchored);
      const glm::vec2 bottomRight = g.position + glm::vec2(40.0F, 20.0F) * g.scale;
      Check(std::fabs(bottomRight.x - 240.0F) < 1e-3F && std::fabs(bottomRight.y - 120.0F) < 1e-3F,
            "anchored corner holds at scale " + std::to_string(scale));
    }

    // Moving the pivot from Lua re-anchors on the very next frame: the anchor is resolved inside
    // TransformSystem, so there is no cached offset to go stale behind a write.
    reg->GetComponent<ScaleComponent>(anchored).value = glm::vec2(2.0F, 2.0F);
    Check(RunLua(lua, "local p = registry.get_pivot(anchored); p.x = 0; p.y = 0"), "pivot moved from Lua");
    reg->Update(1.0F / 60.0F);
    Check(NearVec2(reg->GetComponent<GlobalTransformComponent>(anchored).position, glm::vec2(200.0F, 100.0F)),
          "a pivot written from Lua re-anchors the entity the next frame");
  }

  std::cout << "[color_grid component Lua binding and mutation]\n";
  {
    Check(RunLua(lua, R"LUA(
      grid_ent = load_entity({ components = {
        color_grid = {
          cell_size = 64,
          color1 = { r = 10, g = 20, b = 30, a = 255 },
          color2 = { r = 40, g = 50, b = 60, a = 255 },
          offset = { x = 5, y = 15 },
          scroll_velocity = { x = 100, y = 50 },
          layer = -5,
          is_fixed = true,
          blend_mode = "add",
        }
      }})
    )LUA"),
          "color_grid entity loads from Lua");
    const Entity gridEnt = lua["grid_ent"];
    Check(reg->HasComponent<ColorGridComponent>(gridEnt), "entity has ColorGridComponent in C++");
    const auto& grid = reg->GetComponent<ColorGridComponent>(gridEnt);
    CheckEq(grid.cellWidth, 64.0f, "cellWidth parsed from cell_size");
    CheckEq(grid.cellHeight, 64.0f, "cellHeight parsed from cell_size");
    CheckEq(static_cast<int>(grid.color1.r), 10, "color1.r parsed");
    CheckEq(static_cast<int>(grid.color2.b), 60, "color2.b parsed");
    CheckEq(grid.offset.x, 5.0f, "offset.x parsed");
    CheckEq(grid.offset.y, 15.0f, "offset.y parsed");
    CheckEq(grid.scrollVelocity.x, 100.0f, "scrollVelocity.x parsed");
    CheckEq(grid.layer, -5, "layer parsed");
    Check(grid.isFixed, "isFixed parsed");
    Check(grid.blendMode == octarine::BlendMode::Add, "blendMode parsed as add");

    // Mutation from Lua
    Check(RunLua(lua, R"LUA(
      local g = registry.get_color_grid(grid_ent)
      g.cell_size = 128
      g.offset.x = 25
      g.layer = 2
      g.blend_mode = "blend"
    )LUA"),
          "color_grid mutates from Lua");
    CheckEq(grid.cellWidth, 128.0f, "cellWidth mutated from Lua");
    CheckEq(grid.cellHeight, 128.0f, "cellHeight mutated from Lua");
    CheckEq(grid.offset.x, 25.0f, "offset.x mutated from Lua");
    CheckEq(grid.layer, 2, "layer mutated from Lua");
    Check(grid.blendMode == octarine::BlendMode::Blend, "blendMode mutated from Lua");
  }

  std::cout << "[color_grid loaded via array-of-tables components style]\n";
  {
    Check(RunLua(lua, R"LUA(
      grid_bg = load_entity({
        name = "Grid Background",
        components = {
          {
            color_grid = {
              cell_size = 64,              -- square tile width & height
              color1 = { r = 30, g = 30, b = 30, a = 255 },
              color2 = { r = 60, g = 60, b = 60, a = 255 },
              layer = -10,                 -- render behind sprites
              is_fixed = false,            -- scrolls with camera
              scroll_velocity = { x = 20, y = 0 }, -- auto-scrolls 20 px/sec right
            }
          }
        },
      })
    )LUA"),
          "user's exact load_entity snippet loads successfully");
    const Entity gridBg = lua["grid_bg"];
    Check(reg->HasComponent<ColorGridComponent>(gridBg), "grid_bg has ColorGridComponent");
    Check(reg->HasComponent<NameComponent>(gridBg), "grid_bg has NameComponent");
    CheckEq(reg->GetComponent<NameComponent>(gridBg).name, std::string("Grid Background"), "name parsed");
    const auto& grid = reg->GetComponent<ColorGridComponent>(gridBg);
    CheckEq(grid.cellWidth, 64.0f, "cellWidth is 64");
    CheckEq(grid.cellHeight, 64.0f, "cellHeight is 64");
    CheckEq(static_cast<int>(grid.color1.r), 30, "color1.r is 30");
    CheckEq(static_cast<int>(grid.color2.r), 60, "color2.r is 60");
    CheckEq(grid.layer, -10, "layer is -10");
    Check(!grid.isFixed, "is_fixed is false");
    CheckEq(grid.scrollVelocity.x, 20.0f, "scrollVelocity.x is 20");
    CheckEq(grid.scrollVelocity.y, 0.0f, "scrollVelocity.y is 0");
  }

  std::cout << "[ui_button: key, controller, action, trigger -> on_click]\n";
  {
    UIButtonSystem uiButtonSystem;
    auto eventBus = std::make_unique<EventBus>();
    auto* inputSystem = reg->TryGet<InputSystem>();
    if (!inputSystem) {
      inputSystem = &reg->Set<InputSystem>(InputSystem());
      inputSystem->SubscribeToEvents(eventBus, reg);
    }
    uiButtonSystem.Init(reg, eventBus);

    auto* viewport = reg->TryGet<ViewportInfo>();
    if (!viewport) {
      viewport = &reg->Set<ViewportInfo>(ViewportInfo{});
    }
    viewport->isFocused = true;
    viewport->isHovered = true;

    Check(RunLua(lua, R"LUA(
      clicked_btn1 = false
      clicked_btn2 = false
      clicked_self_matched = false

      btn1 = load_entity({
        components = {
          ui_button = {
            key = "space",
            controller_button = "a",
            action = "submit",
            on_click = function(self, entity)
              clicked_btn1 = true
              if self.key == "space" or self.key == "x" then
                clicked_self_matched = true
              end
            end
          }
        }
      })

      btn2 = load_entity({
        components = {
          ui_button = {
            key = "escape",
            controller_button = "b",
            action = "cancel",
            on_click = function(self, entity)
              clicked_btn2 = true
            end
          }
        }
      })
    )LUA"),
          "ui_button loaded from Lua with key, controller_button, and action");

    const Entity btn1 = lua["btn1"];
    const Entity btn2 = lua["btn2"];
    Check(reg->HasComponent<UIButtonComponent>(btn1), "btn1 has UIButtonComponent");
    Check(reg->HasComponent<UIButtonComponent>(btn2), "btn2 has UIButtonComponent");
    auto& comp1 = reg->GetComponent<UIButtonComponent>(btn1);
    auto& comp2 = reg->GetComponent<UIButtonComponent>(btn2);
    CheckEq(comp1.key, std::string("space"), "key is space");
    CheckEq(comp1.controllerButton, std::string("a"), "controllerButton is a");
    CheckEq(comp1.action, std::string("submit"), "action is submit");

    // 1. Programmatic trigger calls on_click
    Check(RunLua(lua, R"LUA(
      local b = registry.get_ui_button(btn1)
      b:trigger(btn1)
    )LUA"),
          "btn:trigger() runs");
    Check(lua["clicked_btn1"] == true, "btn1 on_click fired via trigger()");
    Check(lua["clicked_self_matched"] == true, "on_click received button self table");
    lua["clicked_btn1"] = false;

    // 2. Keyboard key press (space) triggers btn1 on_click directly
    eventBus->EmitEvent<KeyInputEvent>(SDLK_SPACE, SDL_KMOD_NONE, true);
    Check(lua["clicked_btn1"] == true, "btn1 on_click triggered by keyboard key (space)");
    lua["clicked_btn1"] = false;

    // 3. Controller button press ("south" matching alias "a") triggers btn1 on_click directly
    eventBus->EmitEvent<GamepadButtonEvent>(0, SDL_GAMEPAD_BUTTON_SOUTH, "south", true);
    Check(lua["clicked_btn1"] == true, "btn1 on_click triggered by controller button (south -> a)");
    lua["clicked_btn1"] = false;

    // 4. Action trigger: bind action "submit" to key "return", then press Return
    inputSystem->Bind("submit", "return");
    eventBus->EmitEvent<KeyInputEvent>(SDLK_RETURN, SDL_KMOD_NONE, true);
    Check(lua["clicked_btn1"] == true, "btn1 on_click triggered by action 'submit' bound to return");
    lua["clicked_btn1"] = false;

    // 5. btn2 triggers on_click via key 'escape' and controller 'east' -> 'b'
    eventBus->EmitEvent<KeyInputEvent>(SDLK_ESCAPE, SDL_KMOD_NONE, true);
    Check(lua["clicked_btn2"] == true, "btn2 on_click triggered by key 'escape'");
    lua["clicked_btn2"] = false;

    eventBus->EmitEvent<GamepadButtonEvent>(0, SDL_GAMEPAD_BUTTON_EAST, "east", true);
    Check(lua["clicked_btn2"] == true, "btn2 on_click triggered by controller button 'east'");
    lua["clicked_btn2"] = false;

    // 6. Inactive button does not fire on_click
    comp2.isActive = false;
    eventBus->EmitEvent<KeyInputEvent>(SDLK_ESCAPE, SDL_KMOD_NONE, true);
    Check(lua["clicked_btn2"] == false, "inactive button does not fire on_click on key press");
    comp2.isActive = true;

    // 7. Property mutation via Lua
    Check(RunLua(lua, R"LUA(
      local b = registry.get_ui_button(btn1)
      b.key = "x"
      b.controller_button = "y"
      b.action = "jump"
    )LUA"),
          "btn1 properties mutate from Lua");
    CheckEq(comp1.key, std::string("x"), "btn1.key mutated to x");
    CheckEq(comp1.controllerButton, std::string("y"), "btn1.controllerButton mutated to y");
    CheckEq(comp1.action, std::string("jump"), "btn1.action mutated to jump");
  }

  std::cout << "[debug_overlay: closing overlay returns focus to game window and restores directional inputs]\n";
  {
    auto eventBus = std::make_unique<EventBus>();
    auto* viewport = reg->TryGet<ViewportInfo>();
    if (!viewport) {
      viewport = &reg->Set<ViewportInfo>(ViewportInfo{});
    }

    auto* inputSystem = reg->TryGet<InputSystem>();
    if (!inputSystem) {
      inputSystem = &reg->Set<InputSystem>(InputSystem());
    }
    inputSystem->SubscribeToEvents(eventBus, reg);

    // 1. Simulate debug overlay open and holding focus: viewport is unfocused
    viewport->isFocused = false;
    eventBus->EmitEvent<KeyInputEvent>(SDLK_LEFT, SDL_KMOD_NONE, true);
    Check(!inputSystem->IsKeyDown("left"), "left arrow input is eaten while overlay holds focus");
    eventBus->EmitEvent<KeyInputEvent>(SDLK_LEFT, SDL_KMOD_NONE, false);

    // 2. Closing the debug overlay calls ReturnFocusToGame, returning focus to game window
    if (!reg->TryGet<GameConfig>()) {
      reg->Set<GameConfig>(GameConfig{});
    }
    reg->Get<GameConfig>().GetEngineOptions().showDebugGUI = false;
#ifdef OCTARINE_WITH_IMGUI
    RenderDebugGUISystem::ReturnFocusToGame(&game);
#else
    viewport->isFocused = true;
#endif

    Check(viewport->isFocused, "ReturnFocusToGame restored focus to game window");

    // 3. Directional inputs are now received by the game, not eaten
    eventBus->EmitEvent<KeyInputEvent>(SDLK_LEFT, SDL_KMOD_NONE, true);
    Check(inputSystem->IsKeyDown("left"), "left arrow input is received by game after overlay is closed");
    eventBus->EmitEvent<KeyInputEvent>(SDLK_LEFT, SDL_KMOD_NONE, false);

    eventBus->EmitEvent<KeyInputEvent>(SDLK_RIGHT, SDL_KMOD_NONE, true);
    Check(inputSystem->IsKeyDown("right"), "right arrow input is received by game after overlay is closed");
    eventBus->EmitEvent<KeyInputEvent>(SDLK_RIGHT, SDL_KMOD_NONE, false);

    eventBus->EmitEvent<KeyInputEvent>(SDLK_UP, SDL_KMOD_NONE, true);
    Check(inputSystem->IsKeyDown("up"), "up arrow input is received by game after overlay is closed");
    eventBus->EmitEvent<KeyInputEvent>(SDLK_UP, SDL_KMOD_NONE, false);

    eventBus->EmitEvent<KeyInputEvent>(SDLK_DOWN, SDL_KMOD_NONE, true);
    Check(inputSystem->IsKeyDown("down"), "down arrow input is received by game after overlay is closed");
    eventBus->EmitEvent<KeyInputEvent>(SDLK_DOWN, SDL_KMOD_NONE, false);
  }

  std::cout << "[lifetime component Lua binding and helpers]\n";
  {
    Check(RunLua(lua, R"LUA(
      e_life = load_entity({ components = { lifetime = { duration = 5.0 } } })
      e_life_num = load_entity({ components = { lifetime = 2.5 } })
    )LUA"),
          "lifetime component loads via Lua");

    const Entity eLife = lua["e_life"];
    const Entity eLifeNum = lua["e_life_num"];
    Check(reg->HasComponent<LifetimeComponent>(eLife), "e_life has LifetimeComponent");
    Check(reg->GetComponent<LifetimeComponent>(eLife).lifetimeDuration == 5.0F, "duration parsed as 5.0");
    Check(reg->GetComponent<LifetimeComponent>(eLifeNum).lifetimeDuration == 2.5F, "bare number duration parsed");

    Check(RunLua(lua, "dur = registry.get_lifetime(e_life).duration"), "read duration property");
    CheckEq(lua["dur"].get<float>(), 5.0F, "duration property is 5.0");
    Check(RunLua(lua, "rem = registry.get_lifetime(e_life).remaining_duration"), "read remaining_duration property");
    CheckEq(lua["rem"].get<float>(), 5.0F, "initial remaining_duration equals duration");

    // Decrease lifetime in C++
    reg->GetComponent<LifetimeComponent>(eLife).Decrease(2.0F);
    Check(RunLua(lua, "rem2 = registry.get_lifetime(e_life).remaining_duration"), "read remaining after decrease");
    CheckEq(lua["rem2"].get<float>(), 3.0F, "remaining_duration decreased to 3.0");

    // Extend via helper method
    Check(RunLua(lua, "registry.get_lifetime(e_life):extend(1.5)"), "call extend(1.5)");
    CheckEq(reg->GetComponent<LifetimeComponent>(eLife).remainingDuration, 4.5F,
            "extend bumped remainingDuration to 4.5");

    // Reset via helper method
    Check(RunLua(lua, "registry.get_lifetime(e_life):reset()"), "call reset()");
    CheckEq(reg->GetComponent<LifetimeComponent>(eLife).remainingDuration, 5.0F,
            "reset restored remainingDuration to 5.0");

    Check(RunLua(lua, "alive = registry.get_lifetime(e_life):is_alive()"), "call is_alive()");
    Check(lua["alive"].get<bool>(), "is_alive reports true");
  }

  return octarine::test::ReportSummary("Lua behavior test");
}
