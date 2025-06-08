#pragma once

#include <SDL3/SDL.h>

#include "../Components/AnimationComponent.h"
#include "../Components/SpriteComponent.h"
#include "../ECS/ECS.h"

class AnimationSystem : public System {
 public:
  AnimationSystem() {
    RequireComponent<SpriteComponent>();
    RequireComponent<AnimationComponent>();
  }

  AnimationSystem(const AnimationSystem&) = delete;
  AnimationSystem& operator=(const AnimationSystem&) = delete;

  AnimationSystem(AnimationSystem&&) = delete;
  AnimationSystem& operator=(AnimationSystem&&) = delete;

  ~AnimationSystem() = default;

  void Update(const float deltaTime) const {
    for (auto entity : GetEntities()) {
      auto& animation = entity.GetComponent<AnimationComponent>();
      auto& sprite = entity.GetComponent<SpriteComponent>();

      if (animation.numFrames <= 0 || animation.frameRateSpeed <= 0) {
        continue;
      }

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

      sprite.srcRect.x = static_cast<float>(animation.currentFrame) * sprite.width;
      // sprite.srcRect.y = animation.currentFrame * sprite.height;
    }
  }
};