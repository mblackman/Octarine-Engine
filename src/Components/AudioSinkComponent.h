#pragma once

// Marks an emitter that currently has an audio track, and caches the last-applied spatial/Doppler
// values so SpatialAudioSystem / DopplerSystem only call MIX_Set* when something changed. POD: the
// MIX_Track* itself lives in AudioTrackCache (Systems/AudioTrackCache.h) keyed by entity, so this
// component carries no SDL/MIX types — keeping Components/ free of backend handles.
//
// `finished` latches once the track stops or is stolen back by the pool; a finished sink stays on
// the entity as a "already tried" marker so AudioSystem doesn't re-acquire a track every frame.
struct AudioSinkComponent {
  // lastPan sentinel: any value outside the valid pan range [-1, 1] forces the first stereo update
  // to apply. SpatialAudioSystem resets lastPan to this when a source stops being spatial.
  static constexpr float kPanSentinel = 2.0f;

  bool finished = false;

  // Initial sentinels (lastGain < 0, lastPan = kPanSentinel, lastRatio < 0) force the first frame
  // to apply regardless. `stereoApplied` separately tracks whether MIX_SetTrackStereo has a non-null
  // override currently set; flipping a source from spatial→non-spatial clears it once.
  float lastGain = -1.0f;
  float lastPan = kPanSentinel;
  float lastRatio = -1.0f;
  bool stereoApplied = false;
};
