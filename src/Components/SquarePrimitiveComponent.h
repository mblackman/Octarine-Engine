#pragma once

#include <SDL2/SDL.h>

#include <glm/glm.hpp>

struct SquarePrimitiveComponent {
    glm::vec2 position;
    int layer;
    int width;
    int height;
    SDL_Color color;
    bool isFixed;

    SquarePrimitiveComponent(
        glm::vec2 position = glm::vec2(0, 0),
        int layer = 0,
        int width = 0,
        int height = 0,
        SDL_Color color = {255, 255, 255, 255},
        bool isFixed = true)
        : position(position), layer(layer), width(width), height(height), color(color), isFixed(isFixed) {
    }
};