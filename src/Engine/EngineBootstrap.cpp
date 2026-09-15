#include "Engine/EngineBootstrap.h"

#include <SDL3/SDL.h>

#include <optional>
#include <string>

#include "AssetManager/AssetManager.h"
#include "Audio/AudioTrackCache.h"
#include "Components/CameraComponents.h"
#include "Components/ViewportInfo.h"
#include "ECS/Registry.h"
#include "Engine/EngineContext.h"
#include "Engine/LuaProtect.h"
#include "EventBus/EventBus.h"
#include "Game/Game.h"
#include "Game/GameConfig.h"
#include "General/Logger.h"
#include "General/Rect.h"
#include "Lua/Bindings/RegisterAllBindings.h"
#include "Lua/Modules/RegisterAllModules.h"
#include "Renderer/RenderQueue.h"
#include "Renderer/SpriteRenderCache.h"
#include "Systems/EntityPoolSystem.h"
#include "Systems/InputSystem.h"
#include "Systems/ProjectileEmitSystem.h"

#ifdef OCTARINE_WITH_EDITOR
#include "Editor/Inspectors/RegisterAllInspectors.h"
#endif

#include "Engine/SdlFileReader.h"

namespace {
int LuaDofileViaSDL(lua_State* L) {
  std::size_t plen = 0;
  const char* p = luaL_checklstring(L, 1, &plen);
  const std::string path(p, plen);

  lua_getfield(L, LUA_REGISTRYINDEX, "octarine_assets");
  const auto* assets = static_cast<const AssetManager*>(lua_touserdata(L, -1));
  lua_pop(L, 1);

  auto bytes = ReadFileViaSDL(path, assets);
  if (!bytes) {
    return luaL_error(L, "dofile: cannot open '%s'", path.c_str());
  }
  DecryptLuaBytes(*bytes);

  lua_settop(L, 0);
  const int topBefore = lua_gettop(L);
  const std::string chunkname = "@" + path;
  if (luaL_loadbuffer(L, bytes->data(), bytes->size(), chunkname.c_str()) != 0) {
    return luaL_error(L, "dofile load '%s': %s", path.c_str(), lua_tostring(L, -1));
  }
  if (lua_pcall(L, 0, LUA_MULTRET, 0) != 0) {
    return luaL_error(L, "dofile run '%s': %s", path.c_str(), lua_tostring(L, -1));
  }
  return lua_gettop(L) - topBefore;
}
}  // namespace

namespace engine_bootstrap {
void InstallLuaLibraries(sol::state& lua) {
  lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::string, sol::lib::table);

  // Override Lua's stock `dofile` so chunks load through SDL_IO too. Must run after
  // open_libraries (which installs the default dofile we're replacing).
  lua_State* L = lua.lua_state();
  lua_pushcfunction(L, &LuaDofileViaSDL);
  lua_setglobal(L, "dofile");
}

void InstallCoreSingletons(Registry& registry, EngineContext& context, const int windowWidth, const int windowHeight,
                           const bool withFramePathCaches, sol::state* lua) {
  const octarine::Rect camera{0, 0, static_cast<float>(windowWidth), static_cast<float>(windowHeight)};

  registry.Set<RenderQueue>(RenderQueue());
  registry.Set<CameraComponent>(CameraComponent{camera});
  auto& assetManager = registry.Set<AssetManager>(AssetManager());
  registry.Set<ViewportInfo>(ViewportInfo{0, 0, static_cast<float>(windowWidth), static_cast<float>(windowHeight)});
  if (withFramePathCaches) {
    // Entity-keyed backend-handle caches (SpriteRenderCache for SDL_Texture*,
    // AudioTrackCache for MIX_Track*). Skipped during asset baking.
    registry.Set<SpriteRenderCache>(SpriteRenderCache());
    registry.Set<AudioTrackCache>(AudioTrackCache());
  }

  // Publish the now-live AssetManager onto the context so consumers reach it without a
  // separate Registry::Get<AssetManager>() round-trip. Caller is responsible for the other
  // context fields (sdlRenderer/sdlWindow/eventBus/mixer/config).
  context.assets = &assetManager;

  // Store AssetManager in the Lua registry so C-functions (like LuaDofileViaSDL) can access it
  // and resolve scripts transparently from asset_bundle.pak.
  if (lua != nullptr) {
    lua_State* L = lua->lua_state();
    lua_pushlightuserdata(L, &assetManager);
    lua_setfield(L, LUA_REGISTRYINDEX, "octarine_assets");
  }
}

ProjectileEmitSystem& InstallPoolAndProjectile(Registry& registry) {
  // EntityPoolManager before ProjectileEmitSystem::Init — Init calls
  // Get<EntityPoolManager>().RegisterPool<...>.
  registry.Set<EntityPoolManager>(EntityPoolManager());

  // ProjectileEmitSystem Set + Init so GameModule's fire_projectile binding can capture it
  // from the Registry during the modules-install phase below. Init only does
  // RegisterPool<...> calls — safe before scene/system registration.
  auto& projectileEmitSystem = registry.Set<ProjectileEmitSystem>(ProjectileEmitSystem());
  projectileEmitSystem.Init(registry);
  return projectileEmitSystem;
}

InputSystem& InstallInputSystem(Registry& registry, const std::unique_ptr<EventBus>& eventBus) {
  // InputSystem owned by the Registry so Lua bindings + event subscriptions outlive any
  // single scene script reload. Must be Set before ScriptSystem::CreateLuaBindings so the
  // `input` table is installed alongside ScriptSystem's globals.
  auto& inputSystem = registry.Set<InputSystem>(InputSystem());
  inputSystem.SubscribeToEvents(eventBus, &registry);
  return inputSystem;
}

void RegisterAllComponentBindings() {
  // Component bindings (LuaBinding<T> specializations) must be registered BEFORE
  // ScriptSystem's CreateLuaBindings runs — usertypes + registry.has_/get_ are populated
  // from LuaComponentRegistry at that point.
  RegisterAllLuaBindings();

#ifdef OCTARINE_WITH_EDITOR
  // Editor inspector surface — declarative parallel to the Lua bindings.
  RegisterAllComponentInspectors();
#endif
}

void InstallLuaModules(sol::state& lua, Game& game) { RegisterAllLuaModules(lua, game); }

void SetCommonLuaGlobals(sol::state& lua, const GameConfig& config, const std::string& startupMode) {
  lua["game_window_width"] = config.windowWidth;
  lua["game_window_height"] = config.windowHeight;
  lua["oct_startup_mode"] = startupMode;
}
}  // namespace engine_bootstrap
