#pragma once

#include <SDL3/SDL.h>

#include <cctype>
#include <filesystem>
#include <fstream>
#include <sol/sol.hpp>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "sol_ImGui.h"

#include "../Components/ScriptComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/UIButtonComponent.h"
#include "../EventBus/EventBus.h"
#include "../Events/KeyInputEvent.h"
#include "../Game/Game.h"
#include "../ECS/ECS.h"
#include "../AssetManager/AssetManager.h"

#include "../General/Logger.h"

inline glm::vec2 GetEntityPosition(const Entity entity) {
  if (!entity.HasComponent<TransformComponent>()) {
    Logger::Error("Entity does not have TransformComponent.");
    return {0, 0};
  }

  const auto transform = entity.GetComponent<TransformComponent>();
  return transform.position;
}

inline void SetEntityPosition(const Entity entity, const double x,
                              const double y) {
  if (!entity.HasComponent<TransformComponent>()) {
    Logger::Error("Entity does not have TransformComponent.");
    return;
  }

  auto& transform = entity.GetComponent<TransformComponent>();
  transform.position = glm::vec2(static_cast<float>(x), static_cast<float>(y));
}

inline void SetEntitySpriteSrcRect(const Entity entity, const int srcRectX,
                                   const int srcRectY) {
  if (!entity.HasComponent<SpriteComponent>()) {
    Logger::Error("Entity does not have SpriteComponent.");
    return;
  }

  auto& sprite = entity.GetComponent<SpriteComponent>();
  sprite.srcRect.x = srcRectX;
  sprite.srcRect.y = srcRectY;
}

inline std::vector<std::string> ReadFileLines(const std::string& filename) {
  std::vector<std::string> lines;
  if (std::ifstream file(filename); file.is_open()) {
    std::string line;
    while (std::getline(file, line)) {
      lines.push_back(line);
    }
    file.close();
  } else {
    Logger::Error("Failed to open file: " + filename);
  }
  return lines;
}

inline void SetGameMapDimensions(const double width, const double height) {
  Game::mapHeight = static_cast<float>(height);
  Game::mapWidth = static_cast<float>(width);
}

class ScriptSystem : public System {
public:
  ScriptSystem() {
    RequireComponent<ScriptComponent>();
    keyMap_["ctrl"] = {"left ctrl", "right ctrl"};
    keyMap_["shift"] = {"left shift", "right shift"};
    keyMap_["alt"] = {"left alt", "right alt"};
  }

  ~ScriptSystem() = default;

  void CreateLuaBindings(sol::state& lua, const Game& game) {
    CreateLuaTypes(lua);
    CreateLuaEntityBindings(lua);
    CreateLuaGameBindings(lua, game);
    sol_ImGui::Init(lua);
  }

  void SubscribeToEvents(const std::unique_ptr<EventBus>& eventBus) {
    eventBus->SubscribeEvent<ScriptSystem, KeyInputEvent>(
        this, &ScriptSystem::OnKeyInput);
  }

  void Update(double deltaTime, int elapsedTime) {
    for (auto entity : GetEntities()) {
      auto& script = entity.GetComponent<ScriptComponent>();
      script.updateFunction(entity, deltaTime, elapsedTime);
    }
    pressedKeys_.clear();
  }

private:
  void CreateLuaTypes(sol::state& lua) {
    lua.new_usertype<Entity>("entity", "get_id", &Entity::GetId, "blam",
                             &Entity::Blam, "has_tag", &Entity::HasTag,
                             "in_group", &Entity::InGroup);
    lua.new_usertype<glm::vec2>(
        "vec2",
        sol::constructors<glm::vec2(float, float), glm::vec2()>(),
        "x", &glm::vec2::x,
        "y", &glm::vec2::y);
  }

  void CreateLuaEntityBindings(sol::state& lua) const {
    lua.set_function("get_position", &GetEntityPosition);
    lua.set_function("set_position", &SetEntityPosition);
    lua.set_function("set_sprite_src_rect", &SetEntitySpriteSrcRect);
  }

  void CreateLuaGameBindings(sol::state& lua, const Game& game) {
    lua.set_function("read_file_lines", &ReadFileLines);
    lua.set_function("is_key_pressed", &ScriptSystem::IsKeyPressed, this);
    lua.set_function("is_key_held", &ScriptSystem::IsKeyHeld, this);
    lua.set_function("quit_game", &Game::Quit);
    lua.set_function("set_game_map_dimensions", &SetGameMapDimensions);
    lua.set_function("load_asset",
                     [this, &game](sol::table assetTable) {
                       this->LoadAsset(
                           std::move(assetTable),
                           game.GetAssetManager(),
                           game.GetRenderer());
                     });
    lua.set_function("load_entity", [this, &game](sol::table assetTable) {
      this->LoadEntity(std::move(assetTable), game.GetRegistry());
    });
    lua.set_function("get_asset_path",
                     [this, &game](const std::string& relativePath) {
                       return this->GetAssetPath(
                           relativePath, game.GetAssetManager());
                     });
  }

  bool IsKeyPressed(const std::string& key) {
    if (key.empty()) {
      return false;
    }
    const std::string lowerKey = makeKey(key);

    if (const auto it = keyMap_.find(lowerKey); it != keyMap_.end()) {
      for (const auto& pressedKey : pressedKeys_) {
        if (it->second.find(pressedKey) != it->second.end()) {
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
    const std::string lowerKey = makeKey(key);

    if (const auto it = keyMap_.find(lowerKey); it != keyMap_.end()) {
      for (const auto& heldKey : heldKeys_) {
        if (it->second.find(heldKey) != it->second.end()) {
          return true;
        }
      }
    }
    return heldKeys_.find(lowerKey) != heldKeys_.end();
  }

  void OnKeyInput(KeyInputEvent& event) {
    const std::string key = makeKey(SDL_GetKeyName(event.inputKey));
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

  static std::string makeKey(const std::string& key) {
    std::string lowerKey = key;
    std::transform(lowerKey.begin(), lowerKey.end(), lowerKey.begin(),
                   ::tolower);
    return lowerKey;
  }

  void LoadEntity(sol::table entityTable, Registry* registry) const;

  void LoadAsset(sol::table assetTable,
                 AssetManager* assetManager,
                 SDL_Renderer* renderer) const {
    if (const std::string assetType = assetTable["type"];
      assetType == "texture") {
      assetManager->AddTexture(renderer, assetTable["id"], assetTable["file"]);
    } else if (assetType == "font") {
      const int fontSize = assetTable["font_size"];
      assetManager->AddFont(assetTable["id"], assetTable["file"], fontSize);
    } else {
      Logger::Error("Unknown asset type: " + assetType);
    }
  }

  std::string GetAssetPath(const std::string& relativePath,
                           const AssetManager* assetManager
      ) const {
    return assetManager->GetFullPath(relativePath);
  }

private:
  std::unordered_set<std::string> pressedKeys_;
  std::unordered_set<std::string> heldKeys_;
  std::unordered_map<std::string, std::unordered_set<std::string>> keyMap_;
};

inline void ScriptSystem::LoadEntity(sol::table entityTable, Registry* registry)
const {
  Entity newEntity = registry->CreateEntity();

  // Tag
  if (sol::optional<std::string> tag = entityTable["tag"];
    tag != sol::nullopt) {
    newEntity.Tag(entityTable["tag"]);
  }

  // Group
  if (sol::optional<std::string> group = entityTable["group"];
    group != sol::nullopt) {
    newEntity.Group(entityTable["group"]);
  }

  // Components
  if (sol::optional<sol::table> hasComponents = entityTable["components"];
    hasComponents != sol::nullopt) {
    // Transform
    sol::optional<sol::table> transform =
        entityTable["components"]["transform"];
    if (transform != sol::nullopt) {
      newEntity.AddComponent<TransformComponent>(
          glm::vec2(entityTable["components"]["transform"]["position"]["x"],
                    entityTable["components"]["transform"]["position"]["y"]),
          glm::vec2(
              entityTable["components"]["transform"]["scale"]["x"].
              get_or(1.0),
              entityTable["components"]["transform"]["scale"]["y"].
              get_or(1.0)),
          entityTable["components"]["transform"]["rotation"].get_or(0.0));
    }

    // RigidBody
    sol::optional<sol::table> rigidbody =
        entityTable["components"]["rigidbody"];
    if (rigidbody != sol::nullopt) {
      newEntity.AddComponent<RigidBodyComponent>(glm::vec2(
          entityTable["components"]["rigidbody"]["velocity"]["x"].get_or(0.0),
          entityTable["components"]["rigidbody"]["velocity"]["y"].
          get_or(0.0)));
    }

    // Sprite
    if (sol::optional<sol::table> sprite = entityTable["components"]["sprite"];
      sprite != sol::nullopt) {
      newEntity.AddComponent<SpriteComponent>(
          entityTable["components"]["sprite"]["texture_asset_id"],
          entityTable["components"]["sprite"]["width"],
          entityTable["components"]["sprite"]["height"],
          entityTable["components"]["sprite"]["layer"].get_or(1),
          entityTable["components"]["sprite"]["fixed"].get_or(false),
          entityTable["components"]["sprite"]["src_rect_x"].get_or(0),
          entityTable["components"]["sprite"]["src_rect_y"].get_or(0));
    }

    // Square primitive
    if (sol::optional<sol::table> square = entityTable["components"]["square"];
      square != sol::nullopt) {
      sol::table color = entityTable["components"]["square"]["color"];
      Uint8 alphaDefault = 255;
      SDL_Color sdlColor = {color["r"], color["g"], color["b"],
                            color["a"].get_or(alphaDefault)};
      newEntity.AddComponent<SquarePrimitiveComponent>(
          glm::vec2(
              entityTable["components"]["square"]["position"]["x"].get_or(0),
              entityTable["components"]["square"]["position"]["y"].get_or(0)),
          entityTable["components"]["square"]["layer"].get_or(1),
          entityTable["components"]["square"]["width"],
          entityTable["components"]["square"]["height"], sdlColor,
          entityTable["components"]["square"]["is_fixed"].get_or(false));
    }

    // Animation
    sol::optional<sol::table> animation =
        entityTable["components"]["animation"];
    if (animation != sol::nullopt) {
      newEntity.AddComponent<AnimationComponent>(
          entityTable["components"]["animation"]["num_frames"].get_or(1),
          entityTable["components"]["animation"]["speed_rate"].get_or(1));
    }

    // BoxCollider
    sol::optional<sol::table> collider =
        entityTable["components"]["boxcollider"];
    if (collider != sol::nullopt) {
      newEntity.AddComponent<BoxColliderComponent>(
          entityTable["components"]["boxcollider"]["width"],
          entityTable["components"]["boxcollider"]["height"],
          glm::vec2(
              entityTable["components"]["boxcollider"]["offset"]["x"].
              get_or(0),
              entityTable["components"]["boxcollider"]["offset"]["y"].get_or(
                  0)));
    }

    // Health
    if (sol::optional<sol::table> health = entityTable["components"]["health"];
      health != sol::nullopt) {
      newEntity.AddComponent<HealthComponent>(static_cast<int>(
        entityTable["components"]["health"]["max_health"].get_or(100)));
    }

    // ProjectileEmitter
    sol::optional<sol::table> projectileEmitter =
        entityTable["components"]["projectile_emitter"];
    if (projectileEmitter != sol::nullopt) {
      newEntity.AddComponent<ProjectileEmitterComponent>(
          glm::vec2(entityTable["components"]["projectile_emitter"]
                    ["projectile_velocity"]["x"],
                    entityTable["components"]["projectile_emitter"]
                    ["projectile_velocity"]["y"]),
          1000 *
          entityTable["components"]["projectile_emitter"]
          ["repeat_frequency"]
          .get_or(1),
          1000 *
          entityTable["components"]["projectile_emitter"]["projectile_duration"]
          .get_or(10),
          entityTable["components"]["projectile_emitter"]["hit_damage"].
          get_or(10),
          entityTable["components"]["projectile_emitter"]["friendly"].
          get_or(false));
    }

    // CameraFollow
    sol::optional<sol::table> cameraFollow =
        entityTable["components"]["camera_follow"];
    if (cameraFollow != sol::nullopt) {
      newEntity.AddComponent<CameraFollowComponent>();
    }

    // KeyboardControlled
    sol::optional<sol::table> keyboardControlled =
        entityTable["components"]["keyboard_controller"];
    if (keyboardControlled != sol::nullopt) {
      double velocity =
          entityTable["components"]["keyboard_controller"]["velocity"];
      newEntity.AddComponent<KeyboardControlComponent>(velocity);
    }

    sol::optional<sol::function> onUpdateScript =
        entityTable["components"]["on_update_script"];
    if (onUpdateScript != sol::nullopt) {
      newEntity.AddComponent<ScriptComponent>(onUpdateScript.value());
    }

    if (sol::optional<sol::table> button = entityTable["components"]["button"];
      button != sol::nullopt) {
      sol::table buttonTable = entityTable["components"]["button"];
      bool isActive =
          entityTable["components"]["button"]["is_active"].get_or(false);
      sol::optional<sol::function> onClick =
          entityTable["components"]["button"]["on_click_script"];

      if (onClick != sol::nullopt) {
        newEntity.AddComponent<UIButtonComponent>(isActive, buttonTable,
                                                  onClick.value());
      } else {
        newEntity.AddComponent<UIButtonComponent>(isActive, buttonTable);
      }
    }
  }
}