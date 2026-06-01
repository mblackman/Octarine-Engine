#pragma once

#include <SDL3/SDL.h>

#include "../AssetManager/AssetManager.h"
#include "./RenderQueue.h"

class Registry;

// Owns the off-screen scene target the game renders into each frame, plus the SDL_RenderTarget
// switches + present. Game::Render orchestrates the phases (BeginScene / DrawQueue / EndScene /
// CompositeSceneToWindow / Present) instead of calling SDL_Set*RenderTarget itself; the engine
// keeps `SDL_SetRenderTarget` contained to this TU.
//
// The scene texture is sized to the window at creation time. Lifetime tracks the SDL renderer:
// Game::Initialize calls CreateScene after the renderer exists; Game::Destroy calls DestroyScene
// before SDL_DestroyRenderer. Re-creation on window resize is a follow-up — the existing
// behavior keeps the original window-size target and lets SDL letterbox it.
class Renderer {
 public:
  Renderer() = default;
  ~Renderer() = default;

  Renderer(const Renderer&) = delete;
  Renderer& operator=(const Renderer&) = delete;
  Renderer(Renderer&&) = delete;
  Renderer& operator=(Renderer&&) = delete;

  // Allocate the scene render target. Must run after SDL_CreateRenderer. Returns false (and
  // leaves the renderer in a no-scene state) when SDL_CreateTexture fails — caller logs.
  bool CreateScene(SDL_Renderer* sdlRenderer, int width, int height);

  // Destroy the scene texture. Idempotent; safe to call when CreateScene never ran or failed.
  // Must run before SDL_DestroyRenderer.
  void DestroyScene();

  // Phase 1: bind the scene texture as the render target and clear to the engine's scene-bg
  // grey. Subsequent DrawQueue / per-frame draw calls land in the scene texture.
  void BeginScene(SDL_Renderer* sdlRenderer) const;

  // Phase 2: walk a sorted RenderQueue and dispatch each command to SDL_Render*. Texture/font
  // resolution happens in the producer systems (cmd.texture is already a live handle).
  void DrawQueue(const RenderQueue& renderQueue, SDL_Renderer* sdlRenderer) const;

  // Phase 3: unbind the scene texture (RT becomes the window backbuffer) and clear that
  // backbuffer to black. The editor/debug UI + final composite draw on top of the cleared
  // backbuffer.
  void EndScene(SDL_Renderer* sdlRenderer) const;

  // Player-build composite: blit the full scene texture into the window backbuffer. Editor
  // builds skip this — the Scene window inside ImGui draws the scene texture itself, so
  // composing it to the window again would double-draw.
  void CompositeSceneToWindow(SDL_Renderer* sdlRenderer) const;

  // Phase 4: flip. SDL_RenderPresent stays inside Renderer so the engine has exactly one
  // present site per frame.
  void Present(SDL_Renderer* sdlRenderer) const;

  // Hand the editor / RenderDebugGUI access to the texture they composite into the Scene
  // window. Returns nullptr when CreateScene never ran (headless / bake / pre-Init paths).
  [[nodiscard]] SDL_Texture* GetSceneTexture() const { return scene_texture_; }

 private:
  SDL_Texture* scene_texture_ = nullptr;
};
