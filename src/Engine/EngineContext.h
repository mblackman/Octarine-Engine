#pragma once

// Centralized non-owning bundle of engine-level resources (SDL handles, EventBus,
// AssetManager, GameConfig) accessible to systems and Lua modules as a single typed singleton.
//
// Resource lifetimes are owned by Game (SDL handles), AudioSystem (mixer), and
// Registry singletons (AssetManager and GameConfig). EngineContext holds non-owning pointers.
//
// Forward declarations keep this header free of SDL and SDL_mixer includes.

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
