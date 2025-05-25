#pragma once

#include <SDL2/SDL.h>

struct AnimationComponent
{
    int numFrames;
    int currentFrame;
    int frameRateSpeed;
    bool shouldLoop;
    int startTime;

    AnimationComponent(int numFrames = 1, int frameRateSpeed = 1, bool shouldLoop = true) {
        this->numFrames = numFrames;
        this->currentFrame = 1;
        this->frameRateSpeed = frameRateSpeed;
        this->shouldLoop = shouldLoop;
        this->startTime = SDL_GetTicks();
    }
};
