
#pragma once
#include <sol/sol.hpp>

#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"
#include "Components/AnimationComponent.h"
#include "Components/RigidBodyComponent.h"
#include "Components/SquarePrimitiveComponent.h"
#include "Components/UIButtonComponent.h"

namespace LuaComponentHelpers {
inline glm::vec2 SafeGetVec2(const sol::table& parentTable, const std::string& key, float defaultX = 0.0f,
                             float defaultY = 0.0f) {
  glm::vec2 result(defaultX, defaultY);
  if (sol::optional<sol::table> vecTableOpt = parentTable[key]; vecTableOpt && vecTableOpt.value().valid()) {
    sol::table& vecTable = vecTableOpt.value();
    result.x = vecTable["x"].get_or(defaultX);
    result.y = vecTable["y"].get_or(defaultY);
  }
  return result;
}

inline SDL_Color SafeGetColor(const sol::table& parentTable, const std::string& key, Uint8 defaultR = 0,
                              Uint8 defaultG = 0, Uint8 defaultB = 0, Uint8 defaultA = 0) {
  SDL_Color result = {defaultR, defaultG, defaultB, defaultA};
  if (sol::optional<sol::table> vecTableOpt = parentTable[key]; vecTableOpt && vecTableOpt.value().valid()) {
    sol::table& vecTable = vecTableOpt.value();
    result.r = vecTable["r"].get_or(defaultR);
    result.g = vecTable["g"].get_or(defaultG);
    result.b = vecTable["b"].get_or(defaultB);
    result.a = vecTable["a"].get_or(defaultA);
  }
  return result;
}

template <typename T>
T SafeGetOptionalValue(const sol::table& dataTable, const std::string& key, T defaultValue) {
  return dataTable[key].get_or(defaultValue);
}

inline sol::protected_function SafeGetProtectedFunction(const sol::table& dataTable, const std::string& key) {
  return dataTable[key].get_or(sol::protected_function(sol::lua_nil));
}

inline sol::optional<sol::table> SafeGetOptionalTable(const sol::table& dataTable, const std::string& key) {
  if (dataTable[key].is<sol::table>()) {
    return dataTable[key].get<sol::table>();
  }
  return sol::nullopt;
}

}  // namespace LuaComponentHelpers

class ComponentLuaFactory {
 public:
  static TransformComponent CreateTransformComponent(const sol::table& data) {
    using namespace LuaComponentHelpers;
    const glm::vec2 position = SafeGetVec2(data, "position");
    const glm::vec2 scale = SafeGetVec2(data, "scale", 1.0f, 1.0f);
    const auto rotation = SafeGetOptionalValue<double>(data, "rotation", 0.0);
    return TransformComponent(position, scale, rotation);
  }

  static RigidBodyComponent CreateRigidBodyComponent(const sol::table& data) {
    using namespace LuaComponentHelpers;
    const glm::vec2 velocity = SafeGetVec2(data, "velocity");
    return RigidBodyComponent(velocity);
  }

  static SpriteComponent CreateSpriteComponent(const sol::table& data) {
    using namespace LuaComponentHelpers;
    const std::string textureAssetId = data["texture_asset_id"].get<std::string>();
    const auto width = data["width"].get<float>();
    const auto height = data["height"].get<float>();
    const auto layer = SafeGetOptionalValue<int>(data, "layer", 1);
    const auto fixed = SafeGetOptionalValue<bool>(data, "fixed", false);
    const auto srcRectX = SafeGetOptionalValue<float>(data, "src_rect_x", 0);
    const auto srcRectY = SafeGetOptionalValue<float>(data, "src_rect_y", 0);
    return SpriteComponent(textureAssetId, width, height, layer, fixed, srcRectX, srcRectY);
  }

  static SquarePrimitiveComponent CreateSquarePrimitiveComponent(const sol::table& data) {
    using namespace LuaComponentHelpers;
    const glm::vec2 position = SafeGetVec2(data, "position");
    const auto layer = SafeGetOptionalValue<int>(data, "layer", 1);
    const auto width = SafeGetOptionalValue<float>(data, "width", 0.0f);
    const auto height = SafeGetOptionalValue<float>(data, "height", 0.0f);
    const SDL_Color color = SafeGetColor(data, "color");
    const auto fixed = SafeGetOptionalValue<bool>(data, "fixed", false);
    return SquarePrimitiveComponent(position, layer, width, height, color, fixed);
  }

  static AnimationComponent CreateAnimationComponent(const sol::table& data) {
    using namespace LuaComponentHelpers;
    const int numberFrames = SafeGetOptionalValue<int>(data, "num_frames", 1);
    const int speedRate = SafeGetOptionalValue<int>(data, "speed_rate", 1);
    return AnimationComponent(numberFrames, speedRate);
  }

  static BoxColliderComponent CreateBoxColliderComponent(const sol::table& data) {
    using namespace LuaComponentHelpers;
    const int width = SafeGetOptionalValue<int>(data, "width", 1);
    const int height = SafeGetOptionalValue<int>(data, "height", 1);
    const glm::vec2 offset = SafeGetVec2(data, "offset");
    return BoxColliderComponent(width, height, offset);
  }

  static HealthComponent CreateHealthComponent(const sol::table& data) {
    using namespace LuaComponentHelpers;
    const int maxHealth = SafeGetOptionalValue<int>(data, "max_health", 100);
    return HealthComponent(maxHealth);
  }

  static ProjectileEmitterComponent CreateProjectileEmitterComponent(const sol::table& data) {
    using namespace LuaComponentHelpers;
    const glm::vec2 projectileVelocity = SafeGetVec2(data, "projectile_velocity");
    const int repeatFrequency =
        SafeGetOptionalValue<int>(data, "repeat_frequency", 5) * Constants::kMillisecondsPerSecond;
    const int projectileDuration =
        SafeGetOptionalValue<int>(data, "projectile_duration", 1) * Constants::kMillisecondsPerSecond;
    const int projectileDamage = SafeGetOptionalValue<int>(data, "hit_damage", 10);
    const bool isFriendly = SafeGetOptionalValue<bool>(data, "friendly", false);

    return ProjectileEmitterComponent(projectileVelocity, projectileDuration, repeatFrequency, projectileDamage,
                                      isFriendly);
  }

  static CameraFollowComponent CreateCameraFollowComponent(const sol::table& data) { return {}; }

  static KeyboardControlComponent CreateKeyboardControlledComponent(const sol::table& data) {
    using namespace LuaComponentHelpers;
    const double velocity = data.get<double>("velocity");
    return KeyboardControlComponent(velocity);
  }

  static ScriptComponent CreateScriptComponent(const sol::table& data) {
    using namespace LuaComponentHelpers;
    const sol::protected_function updateFn = SafeGetProtectedFunction(data, "on_update");
    const sol::protected_function onDebugGuiFn = SafeGetProtectedFunction(data, "on_debug_gui");
    return ScriptComponent(updateFn, onDebugGuiFn);
  }

  static UIButtonComponent CreateUIButtonComponent(const sol::table& data) {
    using namespace LuaComponentHelpers;
    const bool isActive = SafeGetOptionalValue<bool>(data, "is_active", true);
    const sol::optional<sol::table> btnTable = SafeGetOptionalTable(data, "button_table");
    const sol::protected_function clickFn = SafeGetProtectedFunction(data, "on_click");
    return UIButtonComponent(isActive, btnTable, clickFn);
  }

  static TextLabelComponent CreateTextLabelComponent(const sol::table& data) {
    using namespace LuaComponentHelpers;
    const glm::vec2 offsetPosition = SafeGetVec2(data, "position");
    const int layer = SafeGetOptionalValue<int>(data, "layer", 1);
    const std::string text = data["text"].get<std::string>();
    const std::string fontId = data["font_id"].get<std::string>();
    const SDL_Color color = SafeGetColor(data, "color");
    const bool isFixed = SafeGetOptionalValue<bool>(data, "is_fixed", true);
    return TextLabelComponent(offsetPosition, layer, text, fontId, color, isFixed);
  }
};
