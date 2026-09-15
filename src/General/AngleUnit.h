#pragma once

#include <cstdint>

// Global unit preference for authored and displayed angles (engine internally uses radians).
namespace octarine {

enum class AngleUnit : std::uint8_t { Radians, Degrees };

constexpr float kRadiansToDegrees = 57.2957795F;
constexpr float kDegreesToRadians = 0.0174532925F;

// Configured during initialization; used across static serialization and binding seams.
class AngleUnits {
 public:
  static void Set(const AngleUnit unit) { unit_ = unit; }
  [[nodiscard]] static AngleUnit Get() { return unit_; }
  [[nodiscard]] static bool IsDegrees() { return unit_ == AngleUnit::Degrees; }
  [[nodiscard]] static const char* Suffix() { return unit_ == AngleUnit::Degrees ? "deg" : "rad"; }

  // Converts authored units to internal radians.
  [[nodiscard]] static float ToRadians(const float authored) {
    return unit_ == AngleUnit::Degrees ? authored * kDegreesToRadians : authored;
  }

  // Converts internal radians to authored units.
  [[nodiscard]] static float ToAuthored(const float radians) {
    return unit_ == AngleUnit::Degrees ? radians * kRadiansToDegrees : radians;
  }

 private:
  static inline AngleUnit unit_ = AngleUnit::Degrees;
};

}  // namespace octarine
