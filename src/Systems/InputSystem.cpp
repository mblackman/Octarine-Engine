#include "Systems/InputSystem.h"

#include <SDL3/SDL.h>

#include <glm/glm.hpp>
#include <string>

#include "Components/ViewportInfo.h"
#include "Game/GameConfig.h"

void InputSystem::BeginFrame() {
  float mx = 0.0f;
  float my = 0.0f;
  SDL_GetMouseState(&mx, &my);

  if (registry_) {
    const auto& viewport = registry_->Get<ViewportInfo>();
    const auto& config = registry_->Get<GameConfig>();
    const glm::vec2 transformed = viewport.TransformCoordinates(mx, my, config.windowWidth, config.windowHeight);
    mouseX_ = transformed.x;
    mouseY_ = transformed.y;
  } else {
    mouseX_ = mx;
    mouseY_ = my;
  }
}

#ifdef OCTARINE_WITH_EDITOR
bool InputSystem::ShouldLogInput() const {
  return registry_ && registry_->Get<GameConfig>().GetEngineOptions().logInputEvents;
}
#endif

void InputSystem::OnKeyInput(const KeyInputEvent& event) {
  if (registry_ && event.isPressed) {
    const auto& viewport = registry_->Get<ViewportInfo>();
    if (!viewport.isFocused) {
      return;
    }
  }

  const std::string key = MakeKey(SDL_GetKeyName(event.inputKey));
#ifdef OCTARINE_WITH_EDITOR
  if (ShouldLogInput()) {
    Logger::Info("[input] key " + std::string(event.isPressed ? "down" : "up") + ": " + key);
  }
#endif
  if (event.isPressed) {
    if (heldKeys_.find(key) == heldKeys_.end()) {
      pressedKeys_.insert(key);
      Dispatch(onKeyDown_, key);
    }
    heldKeys_.insert(key);
  } else {
    heldKeys_.erase(key);
    pressedKeys_.erase(key);
    releasedKeys_.insert(key);
    Dispatch(onKeyUp_, key);
  }
}

void InputSystem::OnMouseInput(const MouseInputEvent& event) {
  if (registry_ && (event.event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)) {
    const auto& viewport = registry_->Get<ViewportInfo>();
    if (!viewport.isHovered) {
      return;
    }
  }

  const int btn = static_cast<int>(event.event.button);
  float x = event.event.x;
  float y = event.event.y;

  if (registry_) {
    const auto& viewport = registry_->Get<ViewportInfo>();
    const auto& config = registry_->Get<GameConfig>();
    const glm::vec2 transformed = viewport.TransformCoordinates(x, y, config.windowWidth, config.windowHeight);
    x = transformed.x;
    y = transformed.y;
  }

#ifdef OCTARINE_WITH_EDITOR
  if (ShouldLogInput()) {
    const bool down = event.event.type == SDL_EVENT_MOUSE_BUTTON_DOWN;
    Logger::Info("[input] mouse " + std::string(down ? "down" : "up") + " btn " + std::to_string(btn) + " at (" +
                 std::to_string(x) + ", " + std::to_string(y) + ")");
  }
#endif

  if (event.event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
    if (heldMouseButtons_.find(btn) == heldMouseButtons_.end()) {
      pressedMouseButtons_.insert(btn);
      Dispatch(onMouseDown_, btn, x, y);
    }
    heldMouseButtons_.insert(btn);
  } else if (event.event.type == SDL_EVENT_MOUSE_BUTTON_UP) {
    heldMouseButtons_.erase(btn);
    pressedMouseButtons_.erase(btn);
    releasedMouseButtons_.insert(btn);
    Dispatch(onMouseUp_, btn, x, y);
  }
}

void InputSystem::OnMouseWheel(const MouseWheelEvent& event) {
  if (registry_) {
    const auto& viewport = registry_->Get<ViewportInfo>();
    if (!viewport.isHovered) {
      return;
    }
  }

#ifdef OCTARINE_WITH_EDITOR
  if (ShouldLogInput()) {
    Logger::Info("[input] wheel " + std::to_string(event.dx) + ", " + std::to_string(event.dy));
  }
#endif

  wheelDx_ += event.dx;
  wheelDy_ += event.dy;
  Dispatch(onMouseWheel_, event.dx, event.dy);
}

bool InputSystem::IsKeyDown(const std::string& key) const {
  if (registry_ && !registry_->Get<ViewportInfo>().isFocused) return false;
  return MatchesSet(key, heldKeys_);
}

bool InputSystem::IsKeyPressed(const std::string& key) const {
  if (registry_ && !registry_->Get<ViewportInfo>().isFocused) return false;
  return MatchesSet(key, pressedKeys_);
}

bool InputSystem::IsKeyReleased(const std::string& key) const {
  if (registry_ && !registry_->Get<ViewportInfo>().isFocused) return false;
  return MatchesSet(key, releasedKeys_);
}

bool InputSystem::MatchesSet(const std::string& key, const std::unordered_set<std::string>& set) const {
  if (key.empty()) return false;
  const std::string lower = MakeKey(key);
  if (const auto it = keyAliases_.find(lower); it != keyAliases_.end()) {
    for (const auto& alias : it->second) {
      if (set.find(alias) != set.end()) return true;
    }
  }
  return set.find(lower) != set.end();
}

bool InputSystem::IsMouseDown(const int btn) const {
  if (registry_ && !registry_->Get<ViewportInfo>().isHovered) return false;
  return btn > 0 && heldMouseButtons_.find(btn) != heldMouseButtons_.end();
}

bool InputSystem::IsMousePressed(const int btn) const {
  if (registry_ && !registry_->Get<ViewportInfo>().isHovered) return false;
  return btn > 0 && pressedMouseButtons_.find(btn) != pressedMouseButtons_.end();
}

bool InputSystem::IsMouseReleased(const int btn) const {
  if (registry_ && !registry_->Get<ViewportInfo>().isHovered) return false;
  return btn > 0 && releasedMouseButtons_.find(btn) != releasedMouseButtons_.end();
}

void InputSystem::Bind(const std::string& action, const std::string& key) {
  auto& keys = actions_[action];
  const std::string lower = MakeKey(key);
  if (std::find(keys.begin(), keys.end(), lower) == keys.end()) {
    keys.push_back(lower);
  }
}

void InputSystem::Unbind(const std::string& action, const std::string& key) {
  const auto it = actions_.find(action);
  if (it == actions_.end()) return;
  const std::string lower = MakeKey(key);
  auto& keys = it->second;
  keys.erase(std::remove(keys.begin(), keys.end(), lower), keys.end());
  if (keys.empty()) actions_.erase(it);
}

bool InputSystem::IsActionDown(const std::string& action) const {
  return AnyBoundKey(action, [this](const std::string& k) { return IsKeyDown(k); });
}

bool InputSystem::IsActionPressed(const std::string& action) const {
  return AnyBoundKey(action, [this](const std::string& k) { return IsKeyPressed(k); });
}

bool InputSystem::IsActionReleased(const std::string& action) const {
  return AnyBoundKey(action, [this](const std::string& k) { return IsKeyReleased(k); });
}
