#pragma once

// Bundle of engine-level resources (SDL handles, EventBus, AssetManager pointer,
// GameConfig pointer) that systems + Lua modules used to fish out of the Registry
// individually as `Registry::Set<SDL_Renderer*>` / `Set<MIX_Mixer*>` / etc.
// Consolidating them into a single typed singleton keeps Registry's singleton-
// component slot count low, makes the engine-level surface obvious, and lets the
// boot sequence be expressed as one assignment instead of three.
//
// Fields are non-owning. Lifetimes belong to Game (SDL handles), AudioSystem
// (mixer), and the Registry singletons (AssetManager + GameConfig, both
// Registry::Set<T>'d). EngineContext only views them.
//
// Forward decls keep this header free of SDL / SDL_mixer includes — consumers
// that actually dereference a field already have the relevant headers on their
// include path, while consumers that only pass the context around stay clean.

struct SDL_Renderer;
struct SDL_Window;
struct MIX_Mixer;

class AssetManager;
class EventBus;
class GameConfig;

struct EngineContext {
  SDL_Renderer* sdlRenderer = nullptr;
  SDL_Window* sdlWindow = nullptr;
  MIX_Mixer* mixer = nullptr;
  EventBus* eventBus = nullptr;
  AssetManager* assets = nullptr;
  GameConfig* config = nullptr;
};
