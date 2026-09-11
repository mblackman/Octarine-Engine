#include "Systems/InputSystem.h"

#include <SDL3/SDL.h>

#include <algorithm>
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
  if (!registry_) return false;
  const auto* config = registry_->TryGet<GameConfig>();
  return config && config->GetEngineOptions().logInputEvents;
}
#endif

void InputSystem::OnKeyInput(const KeyInputEvent& event) {
  if (registry_ && event.isPressed) {
    const auto* viewport = registry_->TryGet<ViewportInfo>();
    if (viewport && !viewport->isFocused) {
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
    if (heldKeys_.erase(key) > 0) {
      pressedKeys_.erase(key);
      releasedKeys_.insert(key);
      Dispatch(onKeyUp_, key);
    }
  }
}

void InputSystem::OnMouseInput(const MouseInputEvent& event) {
  if (registry_ && (event.event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)) {
    const auto* viewport = registry_->TryGet<ViewportInfo>();
    if (viewport && !viewport->isHovered) {
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

void InputSystem::InitGamepadAliases() {
  gamepadAliases_["a"] = {"south", "a", "cross"};
  gamepadAliases_["b"] = {"east", "b", "circle"};
  gamepadAliases_["x"] = {"west", "x", "square"};
  gamepadAliases_["y"] = {"north", "y", "triangle"};
  gamepadAliases_["south"] = {"south", "a", "cross"};
  gamepadAliases_["east"] = {"east", "b", "circle"};
  gamepadAliases_["west"] = {"west", "x", "square"};
  gamepadAliases_["north"] = {"north", "y", "triangle"};
  gamepadAliases_["cross"] = {"south", "a", "cross"};
  gamepadAliases_["circle"] = {"east", "b", "circle"};
  gamepadAliases_["square"] = {"west", "x", "square"};
  gamepadAliases_["triangle"] = {"north", "y", "triangle"};

  gamepadAliases_["lb"] = {"leftshoulder", "lb", "l1", "left shoulder"};
  gamepadAliases_["rb"] = {"rightshoulder", "rb", "r1", "right shoulder"};
  gamepadAliases_["leftshoulder"] = {"leftshoulder", "lb", "l1", "left shoulder"};
  gamepadAliases_["rightshoulder"] = {"rightshoulder", "rb", "r1", "right shoulder"};
  gamepadAliases_["left shoulder"] = {"leftshoulder", "lb", "l1", "left shoulder"};
  gamepadAliases_["right shoulder"] = {"rightshoulder", "rb", "r1", "right shoulder"};
  gamepadAliases_["l1"] = {"leftshoulder", "lb", "l1", "left shoulder"};
  gamepadAliases_["r1"] = {"rightshoulder", "rb", "r1", "right shoulder"};

  gamepadAliases_["ls"] = {"leftstick", "ls", "l3", "left stick"};
  gamepadAliases_["rs"] = {"rightstick", "rs", "r3", "right stick"};
  gamepadAliases_["leftstick"] = {"leftstick", "ls", "l3", "left stick"};
  gamepadAliases_["rightstick"] = {"rightstick", "rs", "r3", "right stick"};
  gamepadAliases_["left stick"] = {"leftstick", "ls", "l3", "left stick"};
  gamepadAliases_["right stick"] = {"rightstick", "rs", "r3", "right stick"};
  gamepadAliases_["l3"] = {"leftstick", "ls", "l3", "left stick"};
  gamepadAliases_["r3"] = {"rightstick", "rs", "r3", "right stick"};

  gamepadAliases_["start"] = {"start", "menu", "options"};
  gamepadAliases_["menu"] = {"start", "menu", "options"};
  gamepadAliases_["options"] = {"start", "menu", "options"};

  gamepadAliases_["back"] = {"back", "select", "share", "view"};
  gamepadAliases_["select"] = {"back", "select", "share", "view"};
  gamepadAliases_["share"] = {"back", "select", "share", "view"};
  gamepadAliases_["view"] = {"back", "select", "share", "view"};

  gamepadAliases_["guide"] = {"guide", "home"};
  gamepadAliases_["home"] = {"guide", "home"};

  gamepadAliases_["dpad_up"] = {"dpup", "dpad_up", "up", "dpad up"};
  gamepadAliases_["dpup"] = {"dpup", "dpad_up", "up", "dpad up"};
  gamepadAliases_["up"] = {"dpup", "dpad_up", "up", "dpad up"};
  gamepadAliases_["dpad up"] = {"dpup", "dpad_up", "up", "dpad up"};

  gamepadAliases_["dpad_down"] = {"dpdown", "dpad_down", "down", "dpad down"};
  gamepadAliases_["dpdown"] = {"dpdown", "dpad_down", "down", "dpad down"};
  gamepadAliases_["down"] = {"dpdown", "dpad_down", "down", "dpad down"};
  gamepadAliases_["dpad down"] = {"dpdown", "dpad_down", "down", "dpad down"};

  gamepadAliases_["dpad_left"] = {"dpleft", "dpad_left", "left", "dpad left"};
  gamepadAliases_["dpleft"] = {"dpleft", "dpad_left", "left", "dpad left"};
  gamepadAliases_["left"] = {"dpleft", "dpad_left", "left", "dpad left"};
  gamepadAliases_["dpad left"] = {"dpleft", "dpad_left", "left", "dpad left"};

  gamepadAliases_["dpad_right"] = {"dpright", "dpad_right", "right", "dpad right"};
  gamepadAliases_["dpright"] = {"dpright", "dpad_right", "right", "dpad right"};
  gamepadAliases_["right"] = {"dpright", "dpad_right", "right", "dpad right"};
  gamepadAliases_["dpad right"] = {"dpright", "dpad_right", "right", "dpad right"};
}

void InputSystem::OnGamepadButton(const GamepadButtonEvent& event) {
  if (registry_ && event.isPressed) {
    const auto* viewport = registry_->TryGet<ViewportInfo>();
    if (viewport && !viewport->isFocused) {
      return;
    }
  }

  const std::string btn = MakeKey(event.buttonName);
  if (btn.empty()) return;

#ifdef OCTARINE_WITH_EDITOR
  if (ShouldLogInput()) {
    Logger::Info("[input] gamepad " + std::string(event.isPressed ? "down" : "up") + ": " + btn);
  }
#endif

  if (event.isPressed) {
    if (heldGamepadButtons_.find(btn) == heldGamepadButtons_.end()) {
      pressedGamepadButtons_.insert(btn);
      Dispatch(onGamepadDown_, btn);
    }
    heldGamepadButtons_.insert(btn);
  } else {
    if (heldGamepadButtons_.erase(btn) > 0) {
      pressedGamepadButtons_.erase(btn);
      releasedGamepadButtons_.insert(btn);
      Dispatch(onGamepadUp_, btn);
    }
  }
}

bool InputSystem::IsKeyDown(const std::string& key) const {
  if (registry_) {
    const auto* viewport = registry_->TryGet<ViewportInfo>();
    if (viewport && !viewport->isFocused) return false;
  }
  return MatchesSet(key, heldKeys_);
}

bool InputSystem::IsKeyPressed(const std::string& key) const {
  if (registry_) {
    const auto* viewport = registry_->TryGet<ViewportInfo>();
    if (viewport && !viewport->isFocused) return false;
  }
  return MatchesSet(key, pressedKeys_);
}

bool InputSystem::IsKeyReleased(const std::string& key) const {
  if (registry_) {
    const auto* viewport = registry_->TryGet<ViewportInfo>();
    if (viewport && !viewport->isFocused) return false;
  }
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

bool InputSystem::IsGamepadButtonDown(const std::string& button) const {
  if (registry_) {
    const auto* viewport = registry_->TryGet<ViewportInfo>();
    if (viewport && !viewport->isFocused) return false;
  }
  return MatchesGamepadSet(button, heldGamepadButtons_);
}

bool InputSystem::IsGamepadButtonPressed(const std::string& button) const {
  if (registry_) {
    const auto* viewport = registry_->TryGet<ViewportInfo>();
    if (viewport && !viewport->isFocused) return false;
  }
  return MatchesGamepadSet(button, pressedGamepadButtons_);
}

bool InputSystem::IsGamepadButtonReleased(const std::string& button) const {
  if (registry_) {
    const auto* viewport = registry_->TryGet<ViewportInfo>();
    if (viewport && !viewport->isFocused) return false;
  }
  return MatchesGamepadSet(button, releasedGamepadButtons_);
}

bool InputSystem::IsGamepadConnected() const { return SDL_HasGamepad(); }

bool InputSystem::MatchesGamepadSet(const std::string& button, const std::unordered_set<std::string>& set) const {
  if (button.empty()) return false;
  const std::string lower = MakeKey(button);
  if (const auto it = gamepadAliases_.find(lower); it != gamepadAliases_.end()) {
    for (const auto& alias : it->second) {
      if (set.find(alias) != set.end()) return true;
    }
  }
  return set.find(lower) != set.end();
}

bool InputSystem::MatchesGamepadAlias(const std::string& requested, const std::string& actual) const {
  if (requested.empty() || actual.empty()) return false;
  const std::string reqLower = MakeKey(requested);
  const std::string actLower = MakeKey(actual);
  if (reqLower == actLower) return true;
  if (const auto it = gamepadAliases_.find(reqLower); it != gamepadAliases_.end()) {
    if (it->second.find(actLower) != it->second.end()) return true;
  }
  if (const auto it = gamepadAliases_.find(actLower); it != gamepadAliases_.end()) {
    if (it->second.find(reqLower) != it->second.end()) return true;
  }
  return false;
}

bool InputSystem::MatchesKeyAlias(const std::string& requested, const std::string& actual) const {
  if (requested.empty() || actual.empty()) return false;
  const std::string reqLower = MakeKey(requested);
  const std::string actLower = MakeKey(actual);
  if (reqLower == actLower) return true;
  if (const auto it = keyAliases_.find(reqLower); it != keyAliases_.end()) {
    if (it->second.find(actLower) != it->second.end()) return true;
  }
  if (const auto it = keyAliases_.find(actLower); it != keyAliases_.end()) {
    if (it->second.find(reqLower) != it->second.end()) return true;
  }
  return false;
}

bool InputSystem::IsActionBoundToKey(const std::string& action, const std::string& key) const {
  const auto it = actions_.find(MakeKey(action));
  if (it == actions_.end()) return false;
  return std::ranges::any_of(it->second, [this, &key](const auto& k) { return MatchesKeyAlias(k, key); });
}

bool InputSystem::IsActionBoundToGamepadButton(const std::string& action, const std::string& button) const {
  const auto it = actions_.find(MakeKey(action));
  if (it == actions_.end()) return false;
  return std::ranges::any_of(it->second, [this, &button](const auto& k) { return MatchesGamepadAlias(k, button); });
}

bool InputSystem::IsActionBoundToInput(const std::string& action, const std::string& input) const {
  return IsActionBoundToKey(action, input) || IsActionBoundToGamepadButton(action, input);
}

bool InputSystem::IsMouseDown(const int btn) const {
  if (registry_) {
    const auto* viewport = registry_->TryGet<ViewportInfo>();
    if (viewport && !viewport->isHovered) return false;
  }
  return btn > 0 && heldMouseButtons_.find(btn) != heldMouseButtons_.end();
}

bool InputSystem::IsMousePressed(const int btn) const {
  if (registry_) {
    const auto* viewport = registry_->TryGet<ViewportInfo>();
    if (viewport && !viewport->isHovered) return false;
  }
  return btn > 0 && pressedMouseButtons_.find(btn) != pressedMouseButtons_.end();
}

bool InputSystem::IsMouseReleased(const int btn) const {
  if (registry_) {
    const auto* viewport = registry_->TryGet<ViewportInfo>();
    if (viewport && !viewport->isHovered) return false;
  }
  return btn > 0 && releasedMouseButtons_.find(btn) != releasedMouseButtons_.end();
}

void InputSystem::Bind(const std::string& action, const std::string& key) {
  auto& keys = actions_[MakeKey(action)];
  const std::string lower = MakeKey(key);
  if (std::find(keys.begin(), keys.end(), lower) == keys.end()) {
    keys.push_back(lower);
  }
}

void InputSystem::Unbind(const std::string& action, const std::string& key) {
  const auto it = actions_.find(MakeKey(action));
  if (it == actions_.end()) return;
  const std::string lower = MakeKey(key);
  auto& keys = it->second;
  keys.erase(std::remove(keys.begin(), keys.end(), lower), keys.end());
  if (keys.empty()) actions_.erase(it);
}

bool InputSystem::IsActionDown(const std::string& action) const {
  return AnyBoundKey(action, [this](const std::string& k) { return IsKeyDown(k) || IsGamepadButtonDown(k); });
}

bool InputSystem::IsActionPressed(const std::string& action) const {
  return AnyBoundKey(action, [this](const std::string& k) { return IsKeyPressed(k) || IsGamepadButtonPressed(k); });
}

bool InputSystem::IsActionReleased(const std::string& action) const {
  return AnyBoundKey(action, [this](const std::string& k) { return IsKeyReleased(k) || IsGamepadButtonReleased(k); });
}
