#pragma once

#include <string>

// Bake-time audio loudness normalizer for `meta.normalize = true` (Stage 14 B2).
//
// Implements ITU-R BS.1770-4 integrated loudness measurement: a K-weighting filter chain
// (pre-filter high-shelf + RLB high-pass) feeds 400 ms gated mean-square blocks at 75% overlap;
// the integrated loudness uses both the absolute (-70 LUFS) + relative (-10 LU below ungated
// mean) gates per spec. Gain is applied uniformly to reach the target loudness; samples that
// would clip int16 are hard-clamped.
//
// Format support is minimal on purpose: PCM 16-bit WAV in / out, mono or stereo, arbitrary
// sample rate (filter coefficients derived per-rate via bilinear transform of the BS.1770 analog
// prototypes). Compressed sources (.ogg / .mp3) are skipped — they need a decoder this engine
// doesn't yet vendor. A future PR can add stb_vorbis + minimp3 to widen the matrix.
class AudioNormalizer {
 public:
  // Read `srcPath`, measure integrated loudness, apply the gain needed to land at
  // `targetLufs`, write the result to `dstPath`. Returns false on unsupported format, I/O error,
  // or a signal too quiet to measure (everything below -70 LUFS absolute gate). On false the
  // caller should fall through to shipping the original.
  static bool NormalizeWav(const std::string& srcPath, const std::string& dstPath, double targetLufs = -16.0);

  // Pure measurement — no file write. Returns the integrated loudness in LUFS, or NaN when the
  // signal is entirely below the -70 LUFS absolute gate. Exposed so the test can verify the
  // post-normalize loudness lands at the target without re-implementing the analyzer.
  static double MeasureWavLufs(const std::string& wavPath);
};
