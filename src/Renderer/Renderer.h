#pragma once

#include <SDL3/SDL.h>

#include <memory>

#include "../AssetManager/AssetManager.h"
#include "./RenderQueue.h"

class Registry;

class Renderer {
 public:
  Renderer() = default;

  Renderer(const Renderer&) = delete;
  Renderer& operator=(const Renderer&) = delete;

  Renderer(Renderer&&) = delete;
  Renderer& operator=(Renderer&&) = delete;

  ~Renderer() = default;

  void Render(const Registry* registry, SDL_Renderer* renderer) const;

 private:
  static void RenderSprite(const Registry* registry, const Entity& entity, SDL_Renderer* renderer,
                           const AssetManager& assetManager, const SDL_FRect& camera);
  static void RenderSquare(const Entity& entity, SDL_Renderer* renderer, const SDL_FRect& camera);
  static void RenderText(const Entity& entity, SDL_Renderer* renderer, const AssetManager& assetManager,
                         const SDL_FRect& camera);
};