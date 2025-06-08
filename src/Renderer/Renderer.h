#pragma once

#include <SDL3/SDL.h>

#include "../AssetManager/AssetManager.h"
#include "./RenderQueue.h"

class Renderer {
 public:
  Renderer() = default;

  Renderer(const Renderer&) = delete;
  Renderer& operator=(const Renderer&) = delete;

  Renderer(Renderer&&) = delete;
  Renderer& operator=(Renderer&&) = delete;

  ~Renderer() = default;

  void Render(const RenderQueue& renderQueue, SDL_Renderer* renderer, const SDL_FRect& camera,
              const std::unique_ptr<AssetManager>& assetManager) const;

 private:
  static void RenderSprite(const Entity& entity, SDL_Renderer* renderer,
                           const std::unique_ptr<AssetManager>& assetManager, const SDL_FRect& camera);
  static void RenderSquare(const Entity& entity, SDL_Renderer* renderer, const SDL_FRect& camera);
  static void RenderText(const Entity& entity, SDL_Renderer* renderer,
                         const std::unique_ptr<AssetManager>& assetManager, const SDL_FRect& camera);
};