#pragma once

#include <glm/vec2.hpp>

struct ViewportInfo {
    float x = 0.0f;
    float y = 0.0f;
    float width = 0.0f;
    float height = 0.0f;
    bool isHovered = true;
    bool isFocused = true;

    [[nodiscard]] glm::vec2 TransformCoordinates(float screenX, float screenY, int targetWidth, int targetHeight) const {
        if (width <= 0.0f || height <= 0.0f) {
            return {screenX, screenY};
        }
        return {
            (screenX - x) * (static_cast<float>(targetWidth) / width),
            (screenY - y) * (static_cast<float>(targetHeight) / height)
        };
    }
};
