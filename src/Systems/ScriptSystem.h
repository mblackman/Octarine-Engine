#pragma once

#include <SDL2/SDL.h>

#include <cctype>
#include <sol/sol.hpp>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "../Components/ScriptComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"
#include "../ECS/ECS.h"
#include "../General/Logger.h"

int GetEntityPosition(Entity entity) {
    if (!entity.HasComponent<TransformComponent>()) {
        Logger::Error("Entity does not have TransformComponent.");
        return 0;
    }

    auto transform = entity.GetComponent<TransformComponent>();
    return transform.position.x;
}

void SetEntityPosition(Entity entity, double x, double y) {
    if (!entity.HasComponent<TransformComponent>()) {
        Logger::Error("Entity does not have TransformComponent.");
        return;
    }

    auto& transform = entity.GetComponent<TransformComponent>();
    transform.position.x = x;
    transform.position.y = y;
}

void SetEntitySpriteSrcRect(Entity entity, int srcRectX, int srcRectY) {
    if (!entity.HasComponent<SpriteComponent>()) {
        Logger::Error("Entity does not have SpriteComponent.");
        return;
    }

    auto& sprite = entity.GetComponent<SpriteComponent>();
    sprite.srcRect.x = srcRectX;
    sprite.srcRect.y = srcRectY;
}

class ScriptSystem : public System {
   public:
    ScriptSystem() : pressedKeys_(), heldKeys_(), keyMap_() {
        RequireComponent<ScriptComponent>();
        keyMap_["ctrl"] = {"left ctrl", "right ctrl"};
        keyMap_["shift"] = {"left shift", "right shift"};
        keyMap_["alt"] = {"left alt", "right alt"};
    }

    ~ScriptSystem() = default;

    void CreateLuaBindings(sol::state& lua) {
        lua.new_usertype<Entity>(
            "entity",
            "get_id", &Entity::GetId,
            "blam", &Entity::Blam,
            "has_tag", &Entity::HasTag,
            "in_group", &Entity::InGroup);
        lua.set_function("get_position", &GetEntityPosition);
        lua.set_function("set_position", &SetEntityPosition);
        lua.set_function("set_sprite_src_rect", &SetEntitySpriteSrcRect);
        lua.set_function("is_key_pressed", &ScriptSystem::IsKeyPressed, this);
        lua.set_function("is_key_held", &ScriptSystem::IsKeyHeld, this);
        lua.set_function("quit_game", &Game::Quit);
    }

    void Update(double deltaTime, int elapsedTime) {
        for (auto entity : GetEntities()) {
            auto& script = entity.GetComponent<ScriptComponent>();
            script.updateFunction(entity, deltaTime, elapsedTime);
        }
        pressedKeys_.clear();
    }

    bool IsKeyPressed(const std::string& key) {
        if (key.empty()) {
            return false;
        }
        std::string lowerKey = makeKey(key);

        auto it = keyMap_.find(lowerKey);

        if (it != keyMap_.end()) {
            for (const auto& key : pressedKeys_) {
                if (it->second.find(key) != it->second.end()) {
                    return true;
                }
            }
        }
        return pressedKeys_.find(lowerKey) != pressedKeys_.end();
    }

    bool IsKeyHeld(const std::string& key) {
        if (key.empty()) {
            return false;
        }
        std::string lowerKey = makeKey(key);

        auto it = keyMap_.find(lowerKey);

        if (it != keyMap_.end()) {
            for (const auto& key : heldKeys_) {
                if (it->second.find(key) != it->second.end()) {
                    return true;
                }
            }
        }
        return heldKeys_.find(lowerKey) != heldKeys_.end();
    }

    void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
        eventBus->SubscribeEvent<ScriptSystem, KeyInputEvent>(this, &ScriptSystem::OnKeyInput);
    }

    void OnKeyInput(KeyInputEvent& event) {
        std::string key = makeKey(SDL_GetKeyName(event.inputKey));
        if (event.isPressed) {
            if (heldKeys_.find(key) == heldKeys_.end()) {
                pressedKeys_.insert(key);
            }
            heldKeys_.insert(key);
        } else {
            heldKeys_.erase(key);
            pressedKeys_.erase(key);
        }
    }

    std::string makeKey(const std::string& key) {
        std::string lowerKey = key;
        std::transform(lowerKey.begin(), lowerKey.end(), lowerKey.begin(), ::tolower);
        return lowerKey;
    }

   private:
    std::unordered_set<std::string> pressedKeys_;
    std::unordered_set<std::string> heldKeys_;
    std::unordered_map<std::string, std::unordered_set<std::string>> keyMap_;
};