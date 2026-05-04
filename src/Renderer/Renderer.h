#pragma once

#include <SDL3/SDL.h>

#include <string>
#include <unordered_map>

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

  void Render(const RenderQueue& renderQueue, SDL_Renderer* renderer) const;
};