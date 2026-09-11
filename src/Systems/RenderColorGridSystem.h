#pragma once

#include <algorithm>
#include <atomic>
#include <cmath>
#include <cstdint>

#include "Components/CameraComponents.h"
#include "Components/ColorGridComponent.h"
#include "Components/GlobalTransformComponent.h"
#include "ECS/Registry.h"
#include "Game/GameConfig.h"
#include "General/Constants.h"
#include "General/PerfUtils.h"
#include "General/Rect.h"
#include "Renderer/RenderCommands.h"
#include "Renderer/RenderQueue.h"

class RenderColorGridSystem {
 public:
  static constexpr uintptr_t kBatchKeyColor1 = 0x100;
  static constexpr uintptr_t kBatchKeyColor2 = 0x200;

  void Prepare(Registry* registry) {
    const auto& gameConfig = registry->Get<GameConfig>();
    camera_ = registry->Get<CameraComponent>().viewport;
    renderQueue_ = &registry->Get<RenderQueue>();
    windowWidth_ = static_cast<float>(gameConfig.windowWidth);
    windowHeight_ = static_cast<float>(gameConfig.windowHeight);
#ifdef OCTARINE_PROFILING
    if (!culledCounter_) culledCounter_ = PROFILE_COUNTER_HANDLE("RenderColorGrid: Culled");
    if (!emplacedCounter_) emplacedCounter_ = PROFILE_COUNTER_HANDLE("RenderColorGrid: Emplaced");
#endif
  }

  void operator()(float dt, ColorGridComponent& grid, const GlobalTransformComponent& transform) const {
    if (grid.scrollVelocity.x != 0.0f || grid.scrollVelocity.y != 0.0f) {
      grid.offset += grid.scrollVelocity * dt;
    }

    if (transform.scale.x == 0.0f || transform.scale.y == 0.0f) return;
    const float cellW = std::max(ColorGridComponent::kMinCellSize, grid.cellWidth * std::abs(transform.scale.x));
    const float cellH = std::max(ColorGridComponent::kMinCellSize, grid.cellHeight * std::abs(transform.scale.y));

    const auto reg = ComputeVisibleRegion(grid, transform, cellW, cellH);
    if (!reg.isVisible) {
      PROFILE_COUNTER_INC(culledCounter_);
      return;
    }

    PROFILE_COUNTER_INC(emplacedCounter_);

    const float originX = transform.position.x + grid.offset.x * transform.scale.x;
    const float originY = transform.position.y + grid.offset.y * transform.scale.y;
    const bool isBounded = (grid.bounds.x > 0.0f && grid.bounds.y > 0.0f);

    if (grid.color1.a == Constants::kUint8Max && grid.color2.a == Constants::kUint8Max) {
      EmplaceOpaqueGrid(grid, transform, reg, cellW, cellH, originX, originY, isBounded);
    } else {
      EmplaceTransparentGrid(grid, transform, reg, cellW, cellH, originX, originY, isBounded);
    }
  }

 private:
  struct GridVisibleRegion {
    float viewL = 0.0f;
    float viewT = 0.0f;
    float viewR = 0.0f;
    float viewB = 0.0f;
    int colMin = 0;
    int colMax = 0;
    int rowMin = 0;
    int rowMax = 0;
    bool isVisible = false;
  };

  [[nodiscard]] GridVisibleRegion ComputeVisibleRegion(const ColorGridComponent& grid,
                                                       const GlobalTransformComponent& transform, const float cellW,
                                                       const float cellH) const {
    GridVisibleRegion reg;
    reg.viewL = grid.isFixed ? 0.0f : camera_.x;
    reg.viewT = grid.isFixed ? 0.0f : camera_.y;
    reg.viewR = grid.isFixed ? windowWidth_ : camera_.x + camera_.w;
    reg.viewB = grid.isFixed ? windowHeight_ : camera_.y + camera_.h;

    const bool isBounded = (grid.bounds.x > 0.0f && grid.bounds.y > 0.0f);
    if (isBounded) {
      const float boundL = transform.position.x;
      const float boundT = transform.position.y;
      const float boundR = boundL + grid.bounds.x * std::abs(transform.scale.x);
      const float boundB = boundT + grid.bounds.y * std::abs(transform.scale.y);

      reg.viewL = std::max(reg.viewL, boundL);
      reg.viewT = std::max(reg.viewT, boundT);
      reg.viewR = std::min(reg.viewR, boundR);
      reg.viewB = std::min(reg.viewB, boundB);
      if (reg.viewL >= reg.viewR || reg.viewT >= reg.viewB) {
        reg.isVisible = false;
        return reg;
      }
    }

    const float originX = transform.position.x + grid.offset.x * transform.scale.x;
    const float originY = transform.position.y + grid.offset.y * transform.scale.y;

    reg.colMin = static_cast<int>(std::floor((reg.viewL - originX) / cellW));
    reg.colMax = static_cast<int>(std::floor((reg.viewR - originX) / cellW));
    reg.rowMin = static_cast<int>(std::floor((reg.viewT - originY) / cellH));
    reg.rowMax = static_cast<int>(std::floor((reg.viewB - originY) / cellH));

    if (isBounded) {
      const int maxCols = static_cast<int>(std::ceil((grid.bounds.x * std::abs(transform.scale.x)) / cellW));
      const int maxRows = static_cast<int>(std::ceil((grid.bounds.y * std::abs(transform.scale.y)) / cellH));
      reg.colMin = std::max(0, reg.colMin);
      reg.colMax = std::min(maxCols - 1, reg.colMax);
      reg.rowMin = std::max(0, reg.rowMin);
      reg.rowMax = std::min(maxRows - 1, reg.rowMax);
      if (reg.colMin > reg.colMax || reg.rowMin > reg.rowMax) {
        reg.isVisible = false;
        return reg;
      }
    }

    reg.isVisible = true;
    return reg;
  }

  struct CellPlacement {
    SDL_FRect destRect{};
    bool visible = false;
  };

  [[nodiscard]] CellPlacement ComputeCellPlacement(const float cellX, const float cellY, const float cellW,
                                                   const float cellH, const GridVisibleRegion& reg,
                                                   const bool isBounded, const bool isFixed) const {
    float x = cellX;
    float y = cellY;
    float w = cellW;
    float h = cellH;
    if (isBounded) {
      const float x2 = std::min(x + w, reg.viewR);
      const float y2 = std::min(y + h, reg.viewB);
      x = std::max(x, reg.viewL);
      y = std::max(y, reg.viewT);
      w = x2 - x;
      h = y2 - y;
      if (w <= 0.0f || h <= 0.0f) {
        return {{}, false};
      }
    }
    const float destX = isFixed ? x : (x - camera_.x);
    const float destY = isFixed ? y : (y - camera_.y);
    return {{destX, destY, w, h}, true};
  }

  void EmplaceOpaqueGrid(const ColorGridComponent& grid, const GlobalTransformComponent& transform,
                         const GridVisibleRegion& reg, const float cellW, const float cellH, const float originX,
                         const float originY, const bool isBounded) const {
    const auto layer = static_cast<unsigned int>(std::max(0, grid.layer));
    const float depth = transform.position.y;
    const SDL_BlendMode sdlBlend = octarine::ToSdlBlendMode(grid.blendMode);

    const float bgX = grid.isFixed ? reg.viewL : (reg.viewL - camera_.x);
    const float bgY = grid.isFixed ? reg.viewT : (reg.viewT - camera_.y);
    const float bgW = reg.viewR - reg.viewL;
    const float bgH = reg.viewB - reg.viewT;

    auto& bgCmd =
        renderQueue_->EmplaceSquare(layer, depth, reinterpret_cast<const void*>(kBatchKeyColor1), grid.blendMode);
    bgCmd.destRect = {bgX, bgY, bgW, bgH};
    bgCmd.color = SDL_Color{grid.color1.r, grid.color1.g, grid.color1.b, grid.color1.a};
    bgCmd.rotation = 0.0f;
    bgCmd.pivot = {0.0f, 0.0f};
    bgCmd.blendMode = sdlBlend;

    const SDL_Color c2{grid.color2.r, grid.color2.g, grid.color2.b, grid.color2.a};
    for (int r = reg.rowMin; r <= reg.rowMax; ++r) {
      for (int c = reg.colMin; c <= reg.colMax; ++c) {
        if (((c & 1) ^ (r & 1)) == 0) continue;

        const float cellX = originX + static_cast<float>(c) * cellW;
        const float cellY = originY + static_cast<float>(r) * cellH;
        const auto placement = ComputeCellPlacement(cellX, cellY, cellW, cellH, reg, isBounded, grid.isFixed);
        if (!placement.visible) continue;

        auto& cmd =
            renderQueue_->EmplaceSquare(layer, depth, reinterpret_cast<const void*>(kBatchKeyColor2), grid.blendMode);
        cmd.destRect = placement.destRect;
        cmd.color = c2;
        cmd.rotation = 0.0f;
        cmd.pivot = {0.0f, 0.0f};
        cmd.blendMode = sdlBlend;
      }
    }
  }

  void EmplaceTransparentGrid(const ColorGridComponent& grid, const GlobalTransformComponent& transform,
                              const GridVisibleRegion& reg, const float cellW, const float cellH, const float originX,
                              const float originY, const bool isBounded) const {
    const auto layer = static_cast<unsigned int>(std::max(0, grid.layer));
    const float depth = transform.position.y;
    const SDL_BlendMode sdlBlend = octarine::ToSdlBlendMode(grid.blendMode);

    const SDL_Color c1{grid.color1.r, grid.color1.g, grid.color1.b, grid.color1.a};
    const SDL_Color c2{grid.color2.r, grid.color2.g, grid.color2.b, grid.color2.a};

    for (int r = reg.rowMin; r <= reg.rowMax; ++r) {
      for (int c = reg.colMin; c <= reg.colMax; ++c) {
        const bool isColor2 = (((c & 1) ^ (r & 1)) != 0);
        const void* batchKey = reinterpret_cast<const void*>(isColor2 ? kBatchKeyColor2 : kBatchKeyColor1);
        const SDL_Color& color = isColor2 ? c2 : c1;

        const float cellX = originX + static_cast<float>(c) * cellW;
        const float cellY = originY + static_cast<float>(r) * cellH;
        const auto placement = ComputeCellPlacement(cellX, cellY, cellW, cellH, reg, isBounded, grid.isFixed);
        if (!placement.visible) continue;

        auto& cmd = renderQueue_->EmplaceSquare(layer, depth, batchKey, grid.blendMode);
        cmd.destRect = placement.destRect;
        cmd.color = color;
        cmd.rotation = 0.0f;
        cmd.pivot = {0.0f, 0.0f};
        cmd.blendMode = sdlBlend;
      }
    }
  }

  RenderQueue* renderQueue_ = nullptr;
  float windowWidth_ = 0.0f;
  float windowHeight_ = 0.0f;
  octarine::Rect camera_{};
#ifdef OCTARINE_PROFILING
  std::atomic<long long>* culledCounter_ = nullptr;
  std::atomic<long long>* emplacedCounter_ = nullptr;
#endif
};
