#pragma once

#include <cstdint>

// Angles are stored and composed in radians everywhere inside the engine. This picks the unit they
// are *authored and displayed* in — Lua component tables, set_rotation(), the rotation usertype,
// the editor inspectors — as a conversion applied at those boundaries. config.ini: AngleUnit=
// radians|degrees, defaulting to degrees.
namespace octarine {

enum class AngleUnit : std::uint8_t { Radians, Degrees };

constexpr float kRadiansToDegrees = 57.2957795F;
constexpr float kDegreesToRadians = 0.0174532925F;

// A global rather than a plumbed dependency because the conversion sites are static binding entry
// points (LuaBinding<T>::fromLua and friends) with no context object to read a config from.
// GameConfig sets it once while parsing config.ini, before any script or scene loads.
class AngleUnits {
 public:
  static void Set(const AngleUnit unit) { unit_ = unit; }
  [[nodiscard]] static AngleUnit Get() { return unit_; }
  [[nodiscard]] static bool IsDegrees() { return unit_ == AngleUnit::Degrees; }
  [[nodiscard]] static const char* Suffix() { return unit_ == AngleUnit::Degrees ? "deg" : "rad"; }

  // Authored value -> the radians the engine stores.
  [[nodiscard]] static float ToRadians(const float authored) {
    return unit_ == AngleUnit::Degrees ? authored * kDegreesToRadians : authored;
  }

  // Stored radians -> the value handed back to whoever authored it.
  [[nodiscard]] static float ToAuthored(const float radians) {
    return unit_ == AngleUnit::Degrees ? radians * kRadiansToDegrees : radians;
  }

 private:
  static inline AngleUnit unit_ = AngleUnit::Degrees;
};

}  // namespace octarine
