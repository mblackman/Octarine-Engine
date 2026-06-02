#pragma once

#include <string>
#include <vector>

class Registry;
struct EngineContext;
struct SDL_Renderer;

// Narrow host interface the Lua free-function modules (Lua/Modules/*) bind against, instead of
// closing over the whole Game. Game implements it; module install fns take LuaBindingContext& so
// the module .cpp files no longer #include "Game/Game.h" — breaking the module -> Game ->
// (re)registers-modules cycle and letting tests supply a fake host.
//
// Surface is intentionally minimal: only what the shipped module bindings actually call. Widen it
// when a binding needs something new, not preemptively.
class LuaBindingContext {
 public:
  virtual ~LuaBindingContext() = default;

  [[nodiscard]] virtual Registry* GetRegistry() const = 0;
  [[nodiscard]] virtual SDL_Renderer* GetRenderer() const = 0;
  [[nodiscard]] virtual EngineContext& GetContext() = 0;

  virtual void LoadScene(const std::string& scenePath) = 0;
  virtual void ReloadScene() = 0;
  virtual void StopScene() = 0;
  virtual void TrackSceneAssets(const std::vector<std::string>& assetIds) = 0;

  [[nodiscard]] virtual bool IsBakeMode() const = 0;
  virtual void RecordBakeValidationFailures(int failures) = 0;

  // Request engine shutdown (the `quit_game` Lua global). Named distinctly from Game's static
  // Quit() so the override doesn't collide with the static member.
  virtual void RequestQuit() = 0;
};
