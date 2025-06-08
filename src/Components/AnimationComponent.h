#pragma once

struct AnimationComponent {
  int numFrames;
  int currentFrame;
  int frameRateSpeed;
  bool shouldLoop;
  double frameTimer;
  bool isFinished;

  explicit AnimationComponent(const int numFrames = 1, const int frameRateSpeed = 1, const bool shouldLoop = true)
      : numFrames(numFrames),
        currentFrame(1),
        frameRateSpeed(frameRateSpeed),
        shouldLoop(shouldLoop),
        frameTimer(0.0),
        isFinished(false) {}
};
