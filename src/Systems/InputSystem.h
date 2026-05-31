#pragma once

#include <SDL3/SDL.h>

#include <algorithm>
#include <cctype>
#include <memory>
#include <sol/sol.hpp>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "../ECS/Registry.h"
#include "../EventBus/EventBus.h"
#include "../Events/KeyInputEvent.h"
#include "../Events/MouseInputEvent.h"
#include "../Events/MouseWheelEvent.h"
#include "../General/Logger.h"

// Per-frame input state machine: held/pressed/released sets for keys + mouse buttons, accumulated
// mouse wheel delta, action map (named action → list of keys), and the script-installed
// callback vectors that fire on each edge. EventBus subscriptions feed OnKeyInput / OnMouseInput
// / OnMouseWheel; the Lua `input.*` surface (is_key_*, on_key_*, bind, etc.) lives in
// `Lua/Bindings/InputSystemLuaBinding.cpp` — InputSystem just exposes the public accessors +
// mutators that binding's lambdas call into.
//
// FOLLOW-UP: the sol::protected_function callback vectors still force this header to pull in
// <sol/sol.hpp>. Closing that out cleanly needs a PImpl-style InputSystemLuaCallbacks struct
// (declared here, defined in InputSystem.cpp) which is more surgery than this stage is scoped
// for. Tracked alongside ArchitectureImprovementsPlan's broader header-sol cleanup.
class InputSystem
{
public:
    InputSystem() : registry_(nullptr)
    {
        keyAliases_["ctrl"] = {"left ctrl", "right ctrl"};
        keyAliases_["shift"] = {"left shift", "right shift"};
        keyAliases_["alt"] = {"left alt", "right alt"};
    }

    static std::string MakeKey(const std::string& key)
    {
        std::string lower = key;
        std::transform(lower.begin(), lower.end(), lower.begin(),
                       [](const unsigned char c) { return static_cast<char>(std::tolower(c)); });
        return lower;
    }

    void SubscribeToEvents(const std::unique_ptr<EventBus>& eventBus, Registry* registry)
    {
        registry_ = registry;
        eventBus->SubscribeEvent<InputSystem, KeyInputEvent>(this, &InputSystem::OnKeyInput);
        eventBus->SubscribeEvent<InputSystem, MouseInputEvent>(this, &InputSystem::OnMouseInput);
        eventBus->SubscribeEvent<InputSystem, MouseWheelEvent>(this, &InputSystem::OnMouseWheel);
    }

    // Called near the top of Game::Update so polling APIs see a fresh cursor position
    // for systems that run this frame. Reads viewport + window dims to map raw SDL coords
    // to game-space (editor's scene-window pan/zoom would alias raw coords).
    void BeginFrame();

    // Called at the end of Game::Update — pressed/released are per-frame edges, wheel
    // delta is a per-frame accumulator.
    void ClearPerFrameInput()
    {
        pressedKeys_.clear();
        releasedKeys_.clear();
        pressedMouseButtons_.clear();
        releasedMouseButtons_.clear();
        wheelDx_ = 0.0f;
        wheelDy_ = 0.0f;
    }

    // Called by Game on scene unload to drop callbacks/bindings owned by the previous scene's
    // scripts. Held state is also cleared so a tap during a transition doesn't stick.
    void ResetLuaState()
    {
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

#ifdef OCTARINE_WITH_EDITOR
    // Editor-only diagnostic logging for input events. Gated behind EngineOptions::logInputEvents so it
    // stays quiet until someone is actually debugging input. Compiled out entirely in player builds.
    [[nodiscard]] bool ShouldLogInput() const;
#endif

    // Accessors used by Lua/Bindings/InputSystemLuaBinding.cpp. Plain getters / mutators so the
    // binding lambdas can read state and append callbacks without poking at internals directly.
    [[nodiscard]] Registry* GetRegistry() const { return registry_; }
    [[nodiscard]] float MouseX() const { return mouseX_; }
    [[nodiscard]] float MouseY() const { return mouseY_; }
    [[nodiscard]] float WheelDx() const { return wheelDx_; }
    [[nodiscard]] float WheelDy() const { return wheelDy_; }

    [[nodiscard]] bool IsKeyDown(const std::string& key) const;
    [[nodiscard]] bool IsKeyPressed(const std::string& key) const;
    [[nodiscard]] bool IsKeyReleased(const std::string& key) const;
    [[nodiscard]] bool IsMouseDown(int btn) const;
    [[nodiscard]] bool IsMousePressed(int btn) const;
    [[nodiscard]] bool IsMouseReleased(int btn) const;
    [[nodiscard]] bool IsActionDown(const std::string& action) const;
    [[nodiscard]] bool IsActionPressed(const std::string& action) const;
    [[nodiscard]] bool IsActionReleased(const std::string& action) const;

    void Bind(const std::string& action, const std::string& key);
    void Unbind(const std::string& action, const std::string& key);
    // Clear all bindings for an action; the Lua `input.unbind(action)` form (no key arg) maps to this.
    void UnbindAction(const std::string& action) { actions_.erase(action); }

    void AddOnKeyDown(sol::protected_function fn) { onKeyDown_.push_back(std::move(fn)); }
    void AddOnKeyUp(sol::protected_function fn) { onKeyUp_.push_back(std::move(fn)); }
    void AddOnMouseDown(sol::protected_function fn) { onMouseDown_.push_back(std::move(fn)); }
    void AddOnMouseUp(sol::protected_function fn) { onMouseUp_.push_back(std::move(fn)); }
    void AddOnMouseWheel(sol::protected_function fn) { onMouseWheel_.push_back(std::move(fn)); }

private:
    void OnKeyInput(const KeyInputEvent& event);
    void OnMouseInput(const MouseInputEvent& event);
    void OnMouseWheel(const MouseWheelEvent& event);

    template <typename... Args>
    static void Dispatch(const std::vector<sol::protected_function>& fns, Args&&... args)
    {
        for (const auto& fn : fns)
        {
            if (fn == sol::lua_nil) continue;
            auto result = fn(std::forward<Args>(args)...);
            if (!result.valid())
            {
                const sol::error err = result;
                Logger::ErrorLua(std::string(err.what()));
            }
        }
    }

    [[nodiscard]] bool MatchesSet(const std::string& key, const std::unordered_set<std::string>& set) const;

    template <typename Pred>
    bool AnyBoundKey(const std::string& action, Pred&& pred) const
    {
        const auto it = actions_.find(action);
        if (it == actions_.end()) return false;
        for (const auto& k : it->second)
        {
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
