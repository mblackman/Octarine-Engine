#pragma once

#include <SDL2/SDL.h>

struct AnimationComponent {
  int numFrames;
  int currentFrame;
  int frameRateSpeed;
  bool shouldLoop;
  int startTime;

  explicit AnimationComponent(int numFrames = 1, int frameRateSpeed = 1,
                     bool shouldLoop = true) : numFrames(numFrames), currentFrame(1), frameRateSpeed(frameRateSpeed), shouldLoop(shouldLoop), startTime(SDL_GetTicks()) {
    
    
    
    
    
  }
};
