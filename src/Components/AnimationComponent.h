#pragma once

#include <SDL3/SDL.h>

struct AnimationComponent {
  int numFrames;
  int currentFrame;
  int frameRateSpeed;
  bool shouldLoop;
  int startTime;

  explicit AnimationComponent(const int numFrames = 1, const int frameRateSpeed = 1, const bool shouldLoop = true)
      : numFrames(numFrames),
        currentFrame(1),
        frameRateSpeed(frameRateSpeed),
        shouldLoop(shouldLoop),
        startTime(SDL_GetTicks()) {}
};
