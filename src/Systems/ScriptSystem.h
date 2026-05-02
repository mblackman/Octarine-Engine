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

#include "../AssetManager/AssetManager.h"
#include "../Components/ScriptComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"
#include "../EventBus/EventBus.h"
#include "../Events/KeyInputEvent.h"
#include "../Game/Game.h"
#include "../General/Logger.h"
#include "Game/GameConfig.h"
#include "Lua/LuaEntityLoader.h"
#include "sol_ImGui.h"

inline glm::vec2 GetEntityPosition(Registry *registry, const Entity entity) {
  if (!registry->HasComponent<TransformComponent>(entity)) {
    Logger::Error("Entity does not have TransformComponent.");
    return {0, 0};
  }

  const auto transform = registry->GetComponent<TransformComponent>(entity);
  return transform.position;
}

inline void SetEntityPosition(Registry *registry, const Entity entity, const double x, const double y) {
  if (!registry->HasComponent<TransformComponent>(entity)) {
    Logger::Error("Entity does not have TransformComponent.");
    return;
  }

  auto transform = registry->GetComponent<TransformComponent>(entity);
  transform.position = glm::vec2(static_cast<float>(x), static_cast<float>(y));
}

inline void SetEntitySpriteSrcRect(Registry *registry, const Entity entity, const float srcRectX,
                                   const float srcRectY) {
  if (!registry->HasComponent<SpriteComponent>(entity)) {
    Logger::Error("Entity does not have SpriteComponent.");
    return;
  }

  auto sprite = registry->GetComponent<SpriteComponent>(entity);
  sprite.srcRect.x = srcRectX;
  sprite.srcRect.y = srcRectY;
}

inline std::vector<std::string> ReadFileLines(const std::string &filename) {
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

inline int LuaHandler(lua_State *lua_state, sol::optional<const std::exception &> exception,
                      const sol::string_view string_view) {
  if (exception) {
    const std::exception &err = exception.value();
    Logger::ErrorLua(std::string(err.what()));
  } else if (!string_view.empty()) {
    Logger::ErrorLua(std::string(string_view.data(), string_view.size()));
  } else {
    Logger::ErrorLua("Unknown Lua Error!");
  }
  return 0;
}

class ScriptSystem {
 public:
  ScriptSystem() {
    keyMap_["ctrl"] = {"left ctrl", "right ctrl"};
    keyMap_["shift"] = {"left shift", "right shift"};
    keyMap_["alt"] = {"left alt", "right alt"};
  }

  void CreateLuaBindings(sol::state &lua, const Game &game) {
    // Configure bindings
    CreateLuaTypes(lua);
    CreateLuaEntityBindings(lua);
    CreateLuaGameBindings(lua, game);

    // Set up logging
    SetupLuaLogging(lua);

    // Init ImGui bindings
    sol_ImGui::Init(lua);
  }

  void SubscribeToEvents(const std::unique_ptr<EventBus> &eventBus) {
    eventBus->SubscribeEvent<ScriptSystem, KeyInputEvent>(this, &ScriptSystem::OnKeyInput);
  }

  void operator()(const ContextFacade &context, ScriptComponent &script) const {
    // for (auto entity : GetEntities()) {
    //   if (auto &script = entity.GetComponent<ScriptComponent>(); script.updateFunction != sol::lua_nil) {
    //     if (auto result = script.updateFunction(script.scriptTable, entity, deltaTime); !result.valid()) {
    //       sol::error err = result;
    //       std::string what = err.what();
    //       Logger::ErrorLua(what);
    //     }
    //   }
    // }
    // pressedKeys_.clear();
  }

 private:
  void CreateLuaTypes(sol::state &lua) {
    // lua.new_usertype<Entity>("entity", "get_id", &Entity::GetId, "blam", &Entity::Blam, "has_tag", &Entity::HasTag,
    //                          "in_group", &Entity::InGroup);
    lua.new_usertype<glm::vec2>("vec2", sol::constructors<glm::vec2(float, float), glm::vec2()>(), "x", &glm::vec2::x,
                                "y", &glm::vec2::y);
  }

  void CreateLuaEntityBindings(sol::state &lua) const {
    lua.set_function("get_position", &GetEntityPosition);
    lua.set_function("set_position", &SetEntityPosition);
    lua.set_function("set_sprite_src_rect", &SetEntitySpriteSrcRect);
  }

  void CreateLuaGameBindings(sol::state &lua, const Game &game) {
    lua.set_function("read_file_lines", &ReadFileLines);
    lua.set_function("is_key_pressed", &ScriptSystem::IsKeyPressed, this);
    lua.set_function("is_key_held", &ScriptSystem::IsKeyHeld, this);
    lua.set_function("quit_game", &Game::Quit);
    lua.set_function("set_game_map_dimensions", [this, &game](const double width, const double height) {
      SetGameMapDimensions(width, height, game);
    });
    lua.set_function("load_asset", [this, &game](sol::table assetTable) {
      this->LoadAsset(std::move(assetTable), game.GetRegistry()->Get<AssetManager>(), game.GetRenderer());
    });
    lua.set_function("load_entity", [this, &game](const sol::table &assetTable) {
      LuaEntityLoader::LoadEntityFromLua(game.GetRegistry(), assetTable);
    });
    lua.set_function("get_asset_path", [this, &game](const std::string &relativePath) {
      return this->GetAssetPath(relativePath, game.GetRegistry()->Get<AssetManager>());
    });
  }

  void SetupLuaLogging(sol::state &lua) const {
    lua.set_function("log", &Logger::LogLua);
    lua.set_function("log_e", &Logger::ErrorLua);
    lua.set_function("log_w", &Logger::WarnLua);
    lua.set_function("log_i", &Logger::InfoLua);

    lua.set_exception_handler(&LuaHandler);

    lua.set_function("print", [](const std::string &message) { Logger::LogLua(message); });
  }

  bool IsKeyPressed(const std::string &key) {
    if (key.empty()) {
      return false;
    }
    const std::string lowerKey = makeKey(key);

    if (const auto it = keyMap_.find(lowerKey); it != keyMap_.end()) {
      for (const auto &pressedKey : pressedKeys_) {
        if (it->second.find(pressedKey) != it->second.end()) {
          return true;
        }
      }
    }
    return pressedKeys_.find(lowerKey) != pressedKeys_.end();
  }

  bool IsKeyHeld(const std::string &key) {
    if (key.empty()) {
      return false;
    }
    const std::string lowerKey = makeKey(key);

    if (const auto it = keyMap_.find(lowerKey); it != keyMap_.end()) {
      for (const auto &heldKey : heldKeys_) {
        if (it->second.find(heldKey) != it->second.end()) {
          return true;
        }
      }
    }
    return heldKeys_.find(lowerKey) != heldKeys_.end();
  }

  void OnKeyInput(const KeyInputEvent &event) {
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

  static std::string makeKey(const std::string &key) {
    std::string lowerKey = key;
    std::transform(lowerKey.begin(), lowerKey.end(), lowerKey.begin(), tolower);
    return lowerKey;
  }

  void LoadAsset(sol::table assetTable, AssetManager &assetManager, SDL_Renderer *renderer) const {
    if (const std::string assetType = assetTable["type"]; assetType == "texture") {
      assetManager.AddTexture(renderer, assetTable["id"], assetTable["file"]);
    } else if (assetType == "font") {
      const float fontSize = assetTable["font_size"];
      assetManager.AddFont(assetTable["id"], assetTable["file"], fontSize);
    } else {
      Logger::Error("Unknown asset type: " + assetType);
    }
  }

  [[nodiscard]] std::string GetAssetPath(const std::string &relativePath, const AssetManager &assetManager) const {
    return assetManager.GetFullPath(relativePath);
  }

  void SetGameMapDimensions(const double width, const double height, const Game &game) {
    auto &gameConfig = game.GetRegistry()->Get<GameConfig>();
    gameConfig.playableAreaHeight = static_cast<float>(height);
    gameConfig.playableAreaWidth = static_cast<float>(width);
  }

  std::unordered_set<std::string> pressedKeys_;
  std::unordered_set<std::string> heldKeys_;
  std::unordered_map<std::string, std::unordered_set<std::string> > keyMap_;
};
