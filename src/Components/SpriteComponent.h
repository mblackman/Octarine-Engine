#pragma once

#include <string>
#include <utility>

#include "General/BlendMode.h"
#include "General/Color.h"
#include "General/Constants.h"
#include "General/Rect.h"
#include "General/SpriteFlip.h"

struct SpriteComponent {
  std::string assetId;
  float width;
  float height;
  int layer;
  bool isFixed;
  octarine::Rect srcRect{};
  octarine::SpriteFlip flip = octarine::SpriteFlip::None;
  // rgb tints (multiplies) the texture, a fades it. 255 across the board = draw unmodified.
  octarine::Color colorMod{Constants::kUint8Max, Constants::kUint8Max, Constants::kUint8Max, Constants::kUint8Max};
  octarine::BlendMode blendMode = octarine::BlendMode::Blend;

  explicit SpriteComponent(std::string t_assetId = "", const float t_width = 0, const float t_height = 0,
                           const int t_layer = 0, const bool t_isFixed = false, const float t_srcRectX = 0,
                           const float t_srcRectY = 0)
      : assetId(std::move(t_assetId)),
        width(t_width),
        height(t_height),
        layer(t_layer),
        isFixed(t_isFixed),
        srcRect{t_srcRectX, t_srcRectY, t_width, t_height} {}
};
