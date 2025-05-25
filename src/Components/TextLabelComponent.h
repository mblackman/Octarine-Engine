#pragma once

#include <SDL2/SDL.h>

#include <glm/glm.hpp>
#include <string>

struct TextLabelComponent {
    glm::vec2 position;
    int layer;
    std::string text;
    std::string fontId;
    SDL_Color color;
    bool isFixed;

    TextLabelComponent(
        glm::vec2 position = glm::vec2(0, 0),
        int layer = 0,
        std::string text = "",
        std::string fontId = "",
        SDL_Color color = {255, 255, 255, 255},
        bool isFixed = true)
        : position(position), layer(layer), text(text), fontId(fontId), color(color), isFixed(isFixed) {
    }
};