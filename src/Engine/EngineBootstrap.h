#pragma once

#include <memory>
#include <sol/sol.hpp>
#include <string>

class EventBus;
class Game;
class GameConfig;
class InputSystem;
class ProjectileEmitSystem;
class Registry;
struct EngineContext;

// Shared boot-sequence phases used by both Game::Setup (full per-frame loop) and
// Game::RunBakeValidation (headless asset bake). Each phase is independent — callers compose
// the subset they need — but the *order* of phases follows the dependency rules baked into
// the engine (e.g. EntityPoolManager must be Set before ProjectileEmitSystem::Init, Lua
// libraries must be open before ScriptSystem::CreateLuaBindings, modules install after
// component bindings).
//
// Why a namespace of free fns instead of a class: there's no state to hold between phases.
// Each fn takes exactly the inputs it needs, returning a reference where one is useful
// downstream (InstallInputSystem → InputSystem& for LuaSystemRegistry registration, etc.).
//
// Before this module existed, Game::Setup and Game::RunBakeValidation duplicated ~50 lines
// of singleton-set + Lua-binding sequence — drift was a matter of when, not if. Centralizing
// the shared spine here means a binding/module added in one place reaches both paths.
namespace engine_bootstrap {
// Open the Lua libs every startup script + module install assumes (base, math, io, string,
// table) and override `dofile` so it loads through SDL_IOFromFile — required for chunks
// loaded out of an APK / .app bundle, harmless on a real filesystem.
void InstallLuaLibraries(sol::state& lua);

// Set the engine-level Registry singletons that the startup script + Lua modules read at
// install time: RenderQueue, CameraComponent (sized to the window), AssetManager,
// ViewportInfo, and — on the live-frame path only (withFramePathCaches; bake skips them since
// no frames render and no audio systems run) — the entity-keyed backend-handle caches
// SpriteRenderCache and AudioTrackCache. Also plumbs the freshly-Set AssetManager pointer onto
// the EngineContext; other context fields must already be populated by the caller.
void InstallCoreSingletons(Registry& registry, EngineContext& context, int windowWidth, int windowHeight,
                           bool withFramePathCaches);

// EntityPoolManager + ProjectileEmitSystem. ProjectileEmitSystem::Init calls RegisterPool
// against the EntityPoolManager, so EntityPoolManager must be Set first. Both bind Lua
// surfaces (fire_projectile), so registration must happen before InstallLuaModules.
// Returns the system reference so callers can stash it for later use.
ProjectileEmitSystem& InstallPoolAndProjectile(Registry& registry);

// InputSystem owned by the Registry + subscribed to its three input events. The `input.*`
// Lua surface is installed later via LuaSystemRegistry::bindAll once Lua libs are open;
// this phase wires the engine-side instance + EventBus subscriptions.
InputSystem& InstallInputSystem(Registry& registry, const std::unique_ptr<EventBus>& eventBus);

// RegisterAllLuaBindings + (when OCTARINE_WITH_EDITOR is on) RegisterAllComponentInspectors.
// Both walk a static registry — idempotent; calling twice is a no-op for the second pass.
// Must run before InstallLuaModules so usertypes exist when modules reference them.
void RegisterAllComponentBindings();

// Invoke RegisterAllLuaModules — installs every free-function module global the startup
// script and scenes reach (load_asset, fire_projectile, load_scene, play_sound, ...).
// Requires the singletons + InputSystem + Lua libs to be live.
void InstallLuaModules(sol::state& lua, Game& game);

// Stash the always-present Lua globals every scene/startup-script reads: window
// dimensions + oct_startup_mode. Pulled out of Setup/Bake so a future startup-mode
// addition only edits one site.
void SetCommonLuaGlobals(sol::state& lua, const GameConfig& config, const std::string& startupMode);
}  // namespace engine_bootstrap
