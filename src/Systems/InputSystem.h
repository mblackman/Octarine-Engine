#pragma once

#include <SDL3/SDL.h>

#include <algorithm>
#include <cctype>
#include <glm/glm.hpp>
#include <memory>
#include <sol/sol.hpp>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "../Components/ViewportInfo.h"
#include "../ECS/Registry.h"
#include "../EventBus/EventBus.h"
#include "../Events/KeyInputEvent.h"
#include "../Events/MouseInputEvent.h"
#include "../Events/MouseWheelEvent.h"
#include "../Game/GameConfig.h"
#include "../General/Logger.h"

class InputSystem {
 public:
  InputSystem() : registry_(nullptr) {
    keyAliases_["ctrl"] = {"left ctrl", "right ctrl"};
    keyAliases_["shift"] = {"left shift", "right shift"};
    keyAliases_["alt"] = {"left alt", "right alt"};
  }

  static std::string MakeKey(const std::string& key) {
    std::string lower = key;
    std::transform(lower.begin(), lower.end(), lower.begin(),
                   [](const unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return lower;
  }

  void SubscribeToEvents(const std::unique_ptr<EventBus>& eventBus, Registry* registry) {
    registry_ = registry;
    eventBus->SubscribeEvent<InputSystem, KeyInputEvent>(this, &InputSystem::OnKeyInput);
    eventBus->SubscribeEvent<InputSystem, MouseInputEvent>(this, &InputSystem::OnMouseInput);
    eventBus->SubscribeEvent<InputSystem, MouseWheelEvent>(this, &InputSystem::OnMouseWheel);
  }

  // Called near the top of Game::Update so polling APIs see a fresh cursor position
  // for systems that run this frame.
  void BeginFrame() {
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

  // Called at the end of Game::Update — pressed/released are per-frame edges, wheel
  // delta is a per-frame accumulator.
  void ClearPerFrameInput() {
    pressedKeys_.clear();
    releasedKeys_.clear();
    pressedMouseButtons_.clear();
    releasedMouseButtons_.clear();
    wheelDx_ = 0.0f;
    wheelDy_ = 0.0f;
  }

  // Called by Game on scene unload to drop callbacks/bindings owned by the previous scene's
  // scripts. Held state is also cleared so a tap during a transition doesn't stick.
  void ResetLuaState() {
    onKeyDown_.clear();
    onKeyUp_.clear();
    onMouseDown_.clear();
    onMouseUp_.clear();
    onMouseWheel_.clear();
    actions_.clear();
    pressedKeys_.clear();
    releasedKeys_.clear();
    heldKeys_.clear();
    pressedMouseButtons_.clear();
    releasedMouseButtons_.clear();
    heldMouseButtons_.clear();
    wheelDx_ = 0.0f;
    wheelDy_ = 0.0f;
  }

  void CreateLuaBindings(sol::state& lua) {
    lua["input"] = lua.create_table();
    sol::table input = lua["input"];

    input.set_function("is_key_down", [this](const std::string& name) { return IsKeyDown(name); });
    input.set_function("is_key_pressed", [this](const std::string& name) { return IsKeyPressed(name); });
    input.set_function("is_key_released", [this](const std::string& name) { return IsKeyReleased(name); });

    input.set_function("is_mouse_down", [this](sol::object btn) { return IsMouseDown(ToMouseButton(btn)); });
    input.set_function("is_mouse_pressed", [this](sol::object btn) { return IsMousePressed(ToMouseButton(btn)); });
    input.set_function("is_mouse_released", [this](sol::object btn) { return IsMouseReleased(ToMouseButton(btn)); });
    input.set_function("is_hovered", [this]() { return registry_ ? registry_->Get<ViewportInfo>().isHovered : true; });
    input.set_function("is_focused", [this]() { return registry_ ? registry_->Get<ViewportInfo>().isFocused : true; });
    input.set_function("mouse_position", [this]() { return glm::vec2(mouseX_, mouseY_); });
    input.set_function("mouse_wheel", [this]() { return glm::vec2(wheelDx_, wheelDy_); });

    input.set_function("bind", [this](const std::string& action, const std::string& key) { Bind(action, key); });
    input.set_function("unbind", [this](const std::string& action, sol::optional<std::string> key) {
      if (key) {
        Unbind(action, *key);
      } else {
        actions_.erase(action);
      }
    });
    input.set_function("is_action_down", [this](const std::string& a) { return IsActionDown(a); });
    input.set_function("is_action_pressed", [this](const std::string& a) { return IsActionPressed(a); });
    input.set_function("is_action_released", [this](const std::string& a) { return IsActionReleased(a); });

    input.set_function("on_key_down", [this](sol::protected_function fn) { onKeyDown_.push_back(std::move(fn)); });
    input.set_function("on_key_up", [this](sol::protected_function fn) { onKeyUp_.push_back(std::move(fn)); });
    input.set_function("on_mouse_down", [this](sol::protected_function fn) { onMouseDown_.push_back(std::move(fn)); });
    input.set_function("on_mouse_up", [this](sol::protected_function fn) { onMouseUp_.push_back(std::move(fn)); });
    input.set_function("on_mouse_wheel",
                       [this](sol::protected_function fn) { onMouseWheel_.push_back(std::move(fn)); });
  }

 private:
  void OnKeyInput(const KeyInputEvent& event) {
    if (registry_ && event.isPressed) {
      const auto& viewport = registry_->Get<ViewportInfo>();
      if (!viewport.isFocused) {
        return;
      }
    }

    const std::string key = MakeKey(SDL_GetKeyName(event.inputKey));
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

  void OnMouseInput(const MouseInputEvent& event) {
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

  void OnMouseWheel(const MouseWheelEvent& event) {
    if (registry_) {
      const auto& viewport = registry_->Get<ViewportInfo>();
      if (!viewport.isHovered) {
        return;
      }
    }

    wheelDx_ += event.dx;
    wheelDy_ += event.dy;
    Dispatch(onMouseWheel_, event.dx, event.dy);
  }

  template <typename... Args>
  static void Dispatch(const std::vector<sol::protected_function>& fns, Args&&... args) {
    for (const auto& fn : fns) {
      if (fn == sol::lua_nil) continue;
      auto result = fn(std::forward<Args>(args)...);
      if (!result.valid()) {
        const sol::error err = result;
        Logger::ErrorLua(std::string(err.what()));
      }
    }
  }

  [[nodiscard]] bool IsKeyDown(const std::string& key) const {
    if (registry_ && !registry_->Get<ViewportInfo>().isFocused) return false;
    return MatchesSet(key, heldKeys_);
  }
  [[nodiscard]] bool IsKeyPressed(const std::string& key) const {
    if (registry_ && !registry_->Get<ViewportInfo>().isFocused) return false;
    return MatchesSet(key, pressedKeys_);
  }
  [[nodiscard]] bool IsKeyReleased(const std::string& key) const {
    if (registry_ && !registry_->Get<ViewportInfo>().isFocused) return false;
    return MatchesSet(key, releasedKeys_);
  }

  [[nodiscard]] bool MatchesSet(const std::string& key, const std::unordered_set<std::string>& set) const {
    if (key.empty()) return false;
    const std::string lower = MakeKey(key);
    if (const auto it = keyAliases_.find(lower); it != keyAliases_.end()) {
      for (const auto& alias : it->second) {
        if (set.find(alias) != set.end()) return true;
      }
    }
    return set.find(lower) != set.end();
  }

  [[nodiscard]] bool IsMouseDown(const int btn) const {
    if (registry_ && !registry_->Get<ViewportInfo>().isHovered) return false;
    return btn > 0 && heldMouseButtons_.find(btn) != heldMouseButtons_.end();
  }
  [[nodiscard]] bool IsMousePressed(const int btn) const {
    if (registry_ && !registry_->Get<ViewportInfo>().isHovered) return false;
    return btn > 0 && pressedMouseButtons_.find(btn) != pressedMouseButtons_.end();
  }
  [[nodiscard]] bool IsMouseReleased(const int btn) const {
    if (registry_ && !registry_->Get<ViewportInfo>().isHovered) return false;
    return btn > 0 && releasedMouseButtons_.find(btn) != releasedMouseButtons_.end();
  }

  static int ToMouseButton(const sol::object& btn) {
    if (btn.is<int>()) return btn.as<int>();
    if (btn.is<std::string>()) {
      const std::string name = MakeKey(btn.as<std::string>());
      if (name == "left") return SDL_BUTTON_LEFT;
      if (name == "middle") return SDL_BUTTON_MIDDLE;
      if (name == "right") return SDL_BUTTON_RIGHT;
      if (name == "x1") return SDL_BUTTON_X1;
      if (name == "x2") return SDL_BUTTON_X2;
    }
    return 0;
  }

  void Bind(const std::string& action, const std::string& key) {
    auto& keys = actions_[action];
    const std::string lower = MakeKey(key);
    if (std::find(keys.begin(), keys.end(), lower) == keys.end()) {
      keys.push_back(lower);
    }
  }

  void Unbind(const std::string& action, const std::string& key) {
    const auto it = actions_.find(action);
    if (it == actions_.end()) return;
    const std::string lower = MakeKey(key);
    auto& keys = it->second;
    keys.erase(std::remove(keys.begin(), keys.end(), lower), keys.end());
    if (keys.empty()) actions_.erase(it);
  }

  [[nodiscard]] bool IsActionDown(const std::string& action) const {
    return AnyBoundKey(action, [this](const std::string& k) { return IsKeyDown(k); });
  }
  [[nodiscard]] bool IsActionPressed(const std::string& action) const {
    return AnyBoundKey(action, [this](const std::string& k) { return IsKeyPressed(k); });
  }
  [[nodiscard]] bool IsActionReleased(const std::string& action) const {
    return AnyBoundKey(action, [this](const std::string& k) { return IsKeyReleased(k); });
  }

  template <typename Pred>
  bool AnyBoundKey(const std::string& action, Pred&& pred) const {
    const auto it = actions_.find(action);
    if (it == actions_.end()) return false;
    for (const auto& k : it->second) {
      if (pred(k)) return true;
    }
    return false;
  }

  Registry* registry_;
  std::unordered_set<std::string> pressedKeys_;
  std::unordered_set<std::string> releasedKeys_;
  std::unordered_set<std::string> heldKeys_;
  std::unordered_map<std::string, std::unordered_set<std::string>> keyAliases_;

  std::unordered_set<int> pressedMouseButtons_;
  std::unordered_set<int> releasedMouseButtons_;
  std::unordered_set<int> heldMouseButtons_;

  float mouseX_ = 0.0f;
  float mouseY_ = 0.0f;
  float wheelDx_ = 0.0f;
  float wheelDy_ = 0.0f;

  std::unordered_map<std::string, std::vector<std::string>> actions_;

  std::vector<sol::protected_function> onKeyDown_;
  std::vector<sol::protected_function> onKeyUp_;
  std::vector<sol::protected_function> onMouseDown_;
  std::vector<sol::protected_function> onMouseUp_;
  std::vector<sol::protected_function> onMouseWheel_;
};
