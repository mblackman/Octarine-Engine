#pragma once

#include <algorithm>
#include <glm/glm.hpp>

#include "General/BlendMode.h"
#include "General/Color.h"
#include "General/Constants.h"

struct ColorGridComponent {
  static constexpr float kMinCellSize = 8.0f;
  static constexpr float kDefaultCellSize = 32.0f;
  static constexpr uint8_t kDefaultColor1Gray = 40;
  static constexpr uint8_t kDefaultColor2Gray = 70;
  static constexpr uint8_t kDefaultAlpha = 255;
  static constexpr octarine::Color kDefaultColor1{kDefaultColor1Gray, kDefaultColor1Gray, kDefaultColor1Gray,
                                                  kDefaultAlpha};
  static constexpr octarine::Color kDefaultColor2{kDefaultColor2Gray, kDefaultColor2Gray, kDefaultColor2Gray,
                                                  kDefaultAlpha};

  glm::vec2 offset{0.0f, 0.0f};
  glm::vec2 scrollVelocity{0.0f, 0.0f};
  glm::vec2 bounds{0.0f, 0.0f};  // (0, 0) = infinite viewport coverage; > 0 = bounded area
  float cellWidth{kDefaultCellSize};
  float cellHeight{kDefaultCellSize};
  octarine::Color color1{kDefaultColor1};
  octarine::Color color2{kDefaultColor2};
  int layer{0};
  bool isFixed{false};
  octarine::BlendMode blendMode{octarine::BlendMode::Blend};

  explicit ColorGridComponent(const float t_cellWidth = kDefaultCellSize, const float t_cellHeight = kDefaultCellSize,
                              const octarine::Color t_color1 = kDefaultColor1,
                              const octarine::Color t_color2 = kDefaultColor2, const int t_layer = 0,
                              const bool t_isFixed = false)
      : cellWidth(std::max(kMinCellSize, t_cellWidth)),
        cellHeight(std::max(kMinCellSize, t_cellHeight)),
        color1(t_color1),
        color2(t_color2),
        layer(t_layer),
        isFixed(t_isFixed) {}

  void SetCellSize(const float size) {
    const float s = std::max(kMinCellSize, size);
    cellWidth = s;
    cellHeight = s;
  }

  [[nodiscard]] float GetCellSize() const { return cellWidth; }
};
