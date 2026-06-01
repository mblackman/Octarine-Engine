#include "AssetManager/AudioNormalizer.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <limits>
#include <vector>

#include "General/Logger.h"

namespace {
constexpr double kPi = 3.14159265358979323846;
constexpr double kAbsoluteGateLufs = -70.0;
constexpr double kRelativeGateLU = 10.0;

// ---- Minimal WAV I/O ----------------------------------------------------------------------------
// Hand-rolls a single subchunk PCM16 RIFF reader/writer. No fmt extensions, no fact chunk, no
// non-PCM formats. Anything that doesn't fit returns false from ReadWav so the bake step skips +
// warns rather than corrupting the file.

#pragma pack(push, 1)
struct WavHeader {
  char riff[4];
  std::uint32_t fileSize;
  char wave[4];
  char fmt[4];
  std::uint32_t fmtSize;
  std::uint16_t audioFormat;  // 1 = PCM
  std::uint16_t numChannels;
  std::uint32_t sampleRate;
  std::uint32_t byteRate;
  std::uint16_t blockAlign;
  std::uint16_t bitsPerSample;
};
#pragma pack(pop)

struct WavData {
  std::uint16_t numChannels{0};
  std::uint32_t sampleRate{0};
  // Interleaved samples in [-1, 1] (channels × frames).
  std::vector<float> samples;
};

bool ReadWav(const std::string& path, WavData& out) {
  std::ifstream f(path, std::ios::binary);
  if (!f) {
    Logger::Warn("AudioNormalizer: cannot open " + path);
    return false;
  }
  WavHeader h{};
  f.read(reinterpret_cast<char*>(&h), sizeof(h));
  if (!f) {
    Logger::Warn("AudioNormalizer: short read on header " + path);
    return false;
  }
  if (std::memcmp(h.riff, "RIFF", 4) != 0 || std::memcmp(h.wave, "WAVE", 4) != 0 ||
      std::memcmp(h.fmt, "fmt ", 4) != 0) {
    Logger::Warn("AudioNormalizer: " + path + " is not a RIFF/WAVE/fmt file");
    return false;
  }
  if (h.audioFormat != 1) {
    Logger::Warn("AudioNormalizer: " + path + " is not PCM (audio_format=" + std::to_string(h.audioFormat) +
                 ") — skipping normalize");
    return false;
  }
  if (h.bitsPerSample != 16) {
    Logger::Warn("AudioNormalizer: " + path + " is not 16-bit PCM (got " + std::to_string(h.bitsPerSample) +
                 ") — skipping normalize");
    return false;
  }
  if (h.numChannels != 1 && h.numChannels != 2) {
    Logger::Warn("AudioNormalizer: " + path + " has " + std::to_string(h.numChannels) +
                 " channels — only mono/stereo supported, skipping");
    return false;
  }
  // The fmt chunk size can be larger than the basic 16-byte WAVEFORMATEX header. Skip the rest.
  if (h.fmtSize > 16) {
    f.seekg(h.fmtSize - 16, std::ios::cur);
  }
  // Walk subchunks until `data` is found — some encoders emit LIST/INFO before data.
  char tag[4];
  std::uint32_t sz = 0;
  while (f.read(tag, 4)) {
    f.read(reinterpret_cast<char*>(&sz), 4);
    if (std::memcmp(tag, "data", 4) == 0) break;
    f.seekg(sz, std::ios::cur);
  }
  if (!f || std::memcmp(tag, "data", 4) != 0) {
    Logger::Warn("AudioNormalizer: " + path + " has no data chunk");
    return false;
  }
  const std::size_t sampleCount = sz / sizeof(std::int16_t);
  std::vector<std::int16_t> raw(sampleCount);
  f.read(reinterpret_cast<char*>(raw.data()), sz);
  if (!f) {
    Logger::Warn("AudioNormalizer: short read on data " + path);
    return false;
  }
  out.numChannels = h.numChannels;
  out.sampleRate = h.sampleRate;
  out.samples.resize(sampleCount);
  constexpr float kScale = 1.0F / 32768.0F;
  for (std::size_t i = 0; i < sampleCount; ++i) {
    out.samples[i] = static_cast<float>(raw[i]) * kScale;
  }
  return true;
}

bool WriteWav(const std::string& path, const WavData& in) {
  std::ofstream f(path, std::ios::binary | std::ios::trunc);
  if (!f) {
    Logger::Error("AudioNormalizer: cannot open " + path + " for writing");
    return false;
  }
  const std::uint32_t dataBytes = static_cast<std::uint32_t>(in.samples.size() * sizeof(std::int16_t));
  WavHeader h{};
  std::memcpy(h.riff, "RIFF", 4);
  h.fileSize = 36 + dataBytes;
  std::memcpy(h.wave, "WAVE", 4);
  std::memcpy(h.fmt, "fmt ", 4);
  h.fmtSize = 16;
  h.audioFormat = 1;
  h.numChannels = in.numChannels;
  h.sampleRate = in.sampleRate;
  h.bitsPerSample = 16;
  h.blockAlign = in.numChannels * 2;
  h.byteRate = in.sampleRate * h.blockAlign;
  f.write(reinterpret_cast<const char*>(&h), sizeof(h));
  f.write("data", 4);
  f.write(reinterpret_cast<const char*>(&dataBytes), 4);
  for (const float s : in.samples) {
    const float scaled = std::clamp(s, -1.0F, 1.0F) * 32767.0F;
    const std::int16_t out_s = static_cast<std::int16_t>(std::lrintf(scaled));
    f.write(reinterpret_cast<const char*>(&out_s), 2);
  }
  return static_cast<bool>(f);
}

// ---- BS.1770 K-weighting biquad coefficients (per sample rate, via RBJ bilinear) ----------------
// Two cascaded biquads: Stage 1 = high-shelf (+4 dB @ 1681.97 Hz, Q ≈ 0.7071); Stage 2 = high-pass
// (≈38.14 Hz, Q ≈ 0.5003). Coefficients normalized so a0 == 1 for the standard direct-form II
// transposed difference equation.
struct Biquad {
  double b0{0}, b1{0}, b2{0}, a1{0}, a2{0};
};

Biquad MakeHighShelf(double fc, double gainDb, double Q, double fs) {
  const double A = std::pow(10.0, gainDb / 40.0);
  const double w0 = 2.0 * kPi * fc / fs;
  const double cosW = std::cos(w0);
  const double sinW = std::sin(w0);
  const double alpha = sinW / (2.0 * Q);
  const double sqrtA2alpha = 2.0 * std::sqrt(A) * alpha;

  const double b0 = A * ((A + 1) + (A - 1) * cosW + sqrtA2alpha);
  const double b1 = -2.0 * A * ((A - 1) + (A + 1) * cosW);
  const double b2 = A * ((A + 1) + (A - 1) * cosW - sqrtA2alpha);
  const double a0 = (A + 1) - (A - 1) * cosW + sqrtA2alpha;
  const double a1 = 2.0 * ((A - 1) - (A + 1) * cosW);
  const double a2 = (A + 1) - (A - 1) * cosW - sqrtA2alpha;
  return {b0 / a0, b1 / a0, b2 / a0, a1 / a0, a2 / a0};
}

Biquad MakeHighPass(double fc, double Q, double fs) {
  const double w0 = 2.0 * kPi * fc / fs;
  const double cosW = std::cos(w0);
  const double sinW = std::sin(w0);
  const double alpha = sinW / (2.0 * Q);

  const double b0 = (1.0 + cosW) / 2.0;
  const double b1 = -(1.0 + cosW);
  const double b2 = (1.0 + cosW) / 2.0;
  const double a0 = 1.0 + alpha;
  const double a1 = -2.0 * cosW;
  const double a2 = 1.0 - alpha;
  return {b0 / a0, b1 / a0, b2 / a0, a1 / a0, a2 / a0};
}

void ApplyBiquadInPlace(std::vector<double>& x, const Biquad& bq) {
  double x1 = 0.0, x2 = 0.0, y1 = 0.0, y2 = 0.0;
  for (double& s : x) {
    const double in = s;
    const double out = bq.b0 * in + bq.b1 * x1 + bq.b2 * x2 - bq.a1 * y1 - bq.a2 * y2;
    x2 = x1;
    x1 = in;
    y2 = y1;
    y1 = out;
    s = out;
  }
}

// Integrated loudness in LUFS for the interleaved PCM in `data`. Returns NaN when the entire
// signal is below the absolute gate (-70 LUFS) — caller treats that as "skip, too quiet to align".
double MeasureLufs(const WavData& data) {
  if (data.samples.empty() || data.sampleRate == 0) {
    return std::numeric_limits<double>::quiet_NaN();
  }

  // K-weighting filter chain (Stage 1 + Stage 2) at the source rate.
  const Biquad stage1 = MakeHighShelf(1681.974450955533, 4.0, 0.7071752369554196, data.sampleRate);
  const Biquad stage2 = MakeHighPass(38.13547087602444, 0.5003270373238773, data.sampleRate);

  // Per-channel K-weighted square energies, interleaved framewise.
  const std::size_t numFrames = data.samples.size() / data.numChannels;
  std::vector<std::vector<double>> chans(data.numChannels, std::vector<double>(numFrames, 0.0));
  for (std::size_t i = 0; i < numFrames; ++i) {
    for (std::uint16_t c = 0; c < data.numChannels; ++c) {
      chans[c][i] = data.samples[i * data.numChannels + c];
    }
  }
  for (auto& ch : chans) {
    ApplyBiquadInPlace(ch, stage1);
    ApplyBiquadInPlace(ch, stage2);
  }

  // 400 ms gated blocks at 75% overlap (step = 100 ms).
  const std::size_t blockSize = static_cast<std::size_t>(0.4 * data.sampleRate);
  const std::size_t hop = static_cast<std::size_t>(0.1 * data.sampleRate);
  if (numFrames < blockSize) {
    return std::numeric_limits<double>::quiet_NaN();
  }
  constexpr std::array<double, 6> kChannelWeights = {1.0, 1.0, 1.0, 1.41, 1.41, 1.0};

  std::vector<double> blockLufs;
  for (std::size_t start = 0; start + blockSize <= numFrames; start += hop) {
    double blockMS = 0.0;
    for (std::uint16_t c = 0; c < data.numChannels; ++c) {
      double sum = 0.0;
      for (std::size_t i = 0; i < blockSize; ++i) {
        const double v = chans[c][start + i];
        sum += v * v;
      }
      const double channelMS = sum / static_cast<double>(blockSize);
      const double w = c < kChannelWeights.size() ? kChannelWeights[c] : 1.0;
      blockMS += w * channelMS;
    }
    if (blockMS > 0.0) {
      blockLufs.push_back(-0.691 + 10.0 * std::log10(blockMS));
    }
  }
  if (blockLufs.empty()) {
    return std::numeric_limits<double>::quiet_NaN();
  }

  // Absolute gate -70 LUFS.
  double absSum = 0.0;
  std::size_t absCount = 0;
  for (const double l : blockLufs) {
    if (l >= kAbsoluteGateLufs) {
      absSum += std::pow(10.0, (l + 0.691) / 10.0);
      ++absCount;
    }
  }
  if (absCount == 0) {
    return std::numeric_limits<double>::quiet_NaN();
  }
  const double ungatedLufs = -0.691 + 10.0 * std::log10(absSum / absCount);
  const double relGate = ungatedLufs - kRelativeGateLU;

  // Relative gate; final integrated loudness from blocks past both thresholds.
  double finalSum = 0.0;
  std::size_t finalCount = 0;
  for (const double l : blockLufs) {
    if (l >= kAbsoluteGateLufs && l >= relGate) {
      finalSum += std::pow(10.0, (l + 0.691) / 10.0);
      ++finalCount;
    }
  }
  if (finalCount == 0) {
    return ungatedLufs;
  }
  return -0.691 + 10.0 * std::log10(finalSum / finalCount);
}

}  // namespace

bool AudioNormalizer::NormalizeWav(const std::string& srcPath, const std::string& dstPath, const double targetLufs) {
  WavData data;
  if (!ReadWav(srcPath, data)) {
    return false;
  }
  const double measured = MeasureLufs(data);
  if (!std::isfinite(measured)) {
    Logger::Warn("AudioNormalizer: " + srcPath + " is below the -70 LUFS absolute gate; skipping normalize");
    return false;
  }
  const double gainDb = targetLufs - measured;
  const double gainLinear = std::pow(10.0, gainDb / 20.0);
  for (float& s : data.samples) {
    const double scaled = static_cast<double>(s) * gainLinear;
    s = static_cast<float>(std::clamp(scaled, -1.0, 1.0));
  }
  if (!WriteWav(dstPath, data)) {
    return false;
  }
  Logger::Info("AudioNormalizer: " + srcPath + " " + std::to_string(measured) + " LUFS -> " +
               std::to_string(targetLufs) + " LUFS (gain " + std::to_string(gainDb) + " dB)");
  return true;
}

double AudioNormalizer::MeasureWavLufs(const std::string& wavPath) {
  WavData data;
  if (!ReadWav(wavPath, data)) {
    return std::numeric_limits<double>::quiet_NaN();
  }
  return MeasureLufs(data);
}
