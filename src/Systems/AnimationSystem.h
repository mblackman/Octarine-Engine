#pragma once

#include <SDL3/SDL.h>

#include "../Components/AnimationComponent.h"
#include "../Components/SpriteComponent.h"

class AnimationSystem {
 public:
  void operator()(const ContextFacade& context, SpriteComponent& sprite, AnimationComponent& animation) const {
    if (animation.numFrames <= 0 || animation.frameRateSpeed <= 0 || animation.isFinished) {
      return;
    }

    UpdateAnimationState(animation, context.DeltaTime());

    sprite.srcRect.x = static_cast<float>(animation.currentFrame) * sprite.width;
    // sprite.srcRect.y = animation.currentFrame * sprite.height;
  }

 private:
  void UpdateAnimationState(AnimationComponent& animation, const float deltaTime) const {
    animation.frameTimer += deltaTime;
    const double timePerFrame = 1.0f / static_cast<double>(animation.frameRateSpeed);

    if (animation.frameTimer >= timePerFrame) {
      const int framesToAdvance = static_cast<int>(std::round(animation.frameTimer / timePerFrame));

      if (animation.shouldLoop) {
        animation.currentFrame = (animation.currentFrame + framesToAdvance) % animation.numFrames;
      } else {
        animation.currentFrame = std::min(animation.currentFrame + framesToAdvance, animation.numFrames - 1);
        if (animation.currentFrame >= animation.numFrames - 1) {
          animation.isFinished = true;
        }
      }

      if (animation.shouldLoop) {
        animation.frameTimer = fmod(animation.frameTimer, timePerFrame);
      } else if (animation.isFinished) {
        animation.frameTimer = 0.0f;
      }
    }
  }
};