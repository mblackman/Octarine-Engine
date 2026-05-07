#pragma once

struct AnimationComponent {
  int numFrames;
  int currentFrame;
  int frameRateSpeed;
  bool shouldLoop;
  double frameTimer;
  bool isFinished;

  explicit AnimationComponent(const int t_numFrames = 1, const int t_frameRateSpeed = 1, const bool t_shouldLoop = true)
      : numFrames(t_numFrames),
        currentFrame(1),
        frameRateSpeed(t_frameRateSpeed),
        shouldLoop(t_shouldLoop),
        frameTimer(0.0),
        isFinished(false) {}
};
