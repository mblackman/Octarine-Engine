#pragma once

#include <glm/glm.hpp>

struct TransformComponent {
  glm::vec2 position;
  glm::vec2 scale;
  double rotation;
  glm::vec2 globalPosition;
  glm::vec2 globalScale;
  double globalRotation;

  explicit TransformComponent(const glm::vec2 position = glm::vec2(0, 0), const glm::vec2 scale = glm::vec2(1, 1),
                              const double rotation = 0.0)
      : position(position),
        scale(scale),
        rotation(rotation),
        globalPosition(position),
        globalScale(scale),
        globalRotation(rotation) {}
};