#pragma once

#include <sol/sol.hpp>

#include "../AssetManager/AssetManager.h"
#include "../ECS/ECS.h"

class ECSLoader {
 public:
  ECSLoader() = default;
  ~ECSLoader() = default;

  static void LoadEntity(sol::table entityTable,
                  const std::unique_ptr<Registry>& registry);
  static void LoadAsset(sol::table assetTable,
                 const std::unique_ptr<AssetManager>& assetManager,
                 SDL_Renderer* renderer);
};