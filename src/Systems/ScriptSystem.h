#pragma once

#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>

#include <filesystem>
#include <fstream>
#include <sol/sol.hpp>
#include <string>
#include <utility>

#include "../AssetManager/AssetManager.h"
#include "../Components/NameComponent.h"
#include "../Components/PositionComponent.h"
#include "../Components/ScriptComponent.h"
#include "../Components/SpriteComponent.h"
#include "../EventBus/EventBus.h"
#include "../Events/AudioPlayEvent.h"
#include "../Game/Game.h"
#include "../General/Logger.h"
#include "Game/GameConfig.h"
#include "Lua/LuaEntityLoader.h"

#ifdef OCTARINE_WITH_IMGUI
#include "sol_ImGui.h"
#endif

inline glm::vec2 GetEntityPosition(Registry* registry, const Entity entity)
{
    if (!registry->HasComponent<PositionComponent>(entity))
    {
        Logger::Error("Entity does not have PositionComponent.");
        return {0, 0};
    }

    return registry->GetComponent<PositionComponent>(entity).value;
}

inline std::string GetEntityName(Registry* registry, const Entity entity)
{
    if (!registry->HasComponent<NameComponent>(entity)) return {};
    return registry->GetComponent<NameComponent>(entity).name;
}

inline void SetEntityName(Registry* registry, const Entity entity, const std::string& name)
{
    registry->AddComponent(entity, NameComponent(name));
}

inline sol::object FindEntityByName(sol::this_state state, Registry* registry, const std::string& name)
{
    for (const auto& entity : registry->GetUserEntities())
    {
        if (!registry->HasComponent<NameComponent>(entity)) continue;
        if (registry->GetComponent<NameComponent>(entity).name == name)
        {
            return sol::make_object(state, entity);
        }
    }
    return sol::lua_nil;
}

inline void SetEntityPosition(Registry* registry, const Entity entity, const double x, const double y)
{
    if (!registry->HasComponent<PositionComponent>(entity))
    {
        Logger::Error("Entity does not have PositionComponent.");
        return;
    }

    registry->GetComponent<PositionComponent>(entity).value =
        glm::vec2(static_cast<float>(x), static_cast<float>(y));
}

inline void SetEntitySpriteSrcRect(Registry* registry, const Entity entity, const float srcRectX,
                                   const float srcRectY)
{
    if (!registry->HasComponent<SpriteComponent>(entity))
    {
        Logger::Error("Entity does not have SpriteComponent.");
        return;
    }

    auto& sprite = registry->GetComponent<SpriteComponent>(entity);
    sprite.srcRect.x = srcRectX;
    sprite.srcRect.y = srcRectY;
}

inline std::vector<std::string> ReadFileLines(const std::string& filename)
{
    std::vector<std::string> lines;
    if (std::ifstream file(filename); file.is_open())
    {
        std::string line;
        while (std::getline(file, line))
        {
            lines.push_back(line);
        }
        file.close();
    }
    else
    {
        Logger::Error("Failed to open file: " + filename);
    }
    return lines;
}

inline int LuaHandler(lua_State* /*lua_state*/, sol::optional<const std::exception&> exception,
                      const sol::string_view string_view)
{
    if (exception)
    {
        const std::exception& err = exception.value();
        Logger::ErrorLua(std::string(err.what()));
    }
    else if (!string_view.empty())
    {
        Logger::ErrorLua(std::string(string_view.data(), string_view.size()));
    }
    else
    {
        Logger::ErrorLua("Unknown Lua Error!");
    }
    return 0;
}

class ScriptSystem
{
public:
    ScriptSystem() = default;

    void CreateLuaBindings(sol::state& lua, const Game& game)
    {
        // Configure bindings
        CreateLuaTypes(lua);
        CreateLuaGameBindings(lua, game);

        // Set up logging
        SetupLuaLogging(lua);

#ifdef OCTARINE_WITH_IMGUI
        // Init ImGui bindings
        sol_ImGui::Init(lua);
#endif
    }

    void operator()(const ContextFacade& context, ScriptComponent& script) const
    {
        if (script.updateFunction == sol::lua_nil)
        {
            return;
        }

        if (auto result = script.updateFunction(script.scriptTable, context.GetEntity(), context.GetDeltaTime());
            !result.valid())
        {
            const sol::error err = result;
            Logger::ErrorLua(std::string(err.what()));
        }
    }

private:
    void CreateLuaTypes(sol::state& lua)
    {
        lua.new_usertype<Entity>("entity", "get_id", &Entity::GetId);
        lua.new_usertype<glm::vec2>("vec2", sol::constructors<glm::vec2(float, float), glm::vec2()>(), "x",
                                    &glm::vec2::x,
                                    "y", &glm::vec2::y);
    }

    void CreateLuaGameBindings(sol::state& lua, const Game& game)
    {
        lua.set_function("read_file_lines", &ReadFileLines);
        lua.set_function("quit_game", &Game::Quit);
        lua.set_function("blam", [&game](const Entity entity) { game.GetRegistry()->BlamEntity(entity); });
        lua.set_function("get_name", [&game](const Entity entity)
        {
            return GetEntityName(game.GetRegistry(), entity);
        });
        lua.set_function("set_name", [&game](const Entity entity, const std::string& name)
        {
            SetEntityName(game.GetRegistry(), entity, name);
        });
        lua.set_function("find_entity_by_name", [&game](sol::this_state state, const std::string& name)
        {
            return FindEntityByName(state, game.GetRegistry(), name);
        });
        lua.set_function("get_position",
                         [&game](const Entity entity) { return GetEntityPosition(game.GetRegistry(), entity); });
        lua.set_function("set_position", [&game](const Entity entity, const double x, const double y)
        {
            SetEntityPosition(game.GetRegistry(), entity, x, y);
        });
        lua.set_function("set_sprite_src_rect", [&game](const Entity entity, const float x, const float y)
        {
            SetEntitySpriteSrcRect(game.GetRegistry(), entity, x, y);
        });
        lua.set_function("set_game_map_dimensions", [this, &game](const double width, const double height)
        {
            SetGameMapDimensions(width, height, game);
        });
        lua.set_function("get_game_map_dimensions", [&game]()
        {
            const auto& gameConfig = game.GetRegistry()->Get<GameConfig>();
            return glm::vec2(gameConfig.playableAreaWidth, gameConfig.playableAreaHeight);
        });
        lua.set_function("load_asset", [this, &game](sol::table assetTable)
        {
            auto* registry = game.GetRegistry();
            this->LoadAsset(std::move(assetTable), registry->Get<AssetManager>(), game.GetRenderer(),
                            registry->Get<MIX_Mixer*>());
        });
        lua.set_function("load_entity", [&game](const sol::table& assetTable)
        {
            LuaEntityLoader::LoadEntityFromLua(game.GetRegistry(), assetTable);
        });
        lua.set_function("clear_scene", [&game]() { const_cast<Game&>(game).StopScene(); });
        lua.set_function("load_scene", [&game](const std::string& path) { const_cast<Game&>(game).LoadScene(path); });
        lua.set_function("reload_scene", [&game]() { const_cast<Game&>(game).ReloadScene(); });
        lua.set_function("stop_scene", [&game]() { const_cast<Game&>(game).StopScene(); });

        lua.set_function("get_asset_path", [this, &game](const std::string& relativePath)
        {
            return this->GetAssetPath(relativePath, game.GetRegistry()->Get<AssetManager>());
        });
        lua.set_function("play_sound", [&game](const std::string& clipId, const sol::optional<float> volume)
        {
            auto* eventBus = game.GetRegistry()->Get<EventBus*>();
            if (!eventBus)
            {
                Logger::Error("play_sound called before event bus is ready");
                return;
            }
            eventBus->EmitEvent<AudioPlayEvent>(clipId, volume.value_or(1.0F));
        });
    }

    void SetupLuaLogging(sol::state& lua) const
    {
        lua.set_function("log", &Logger::LogLua);
        lua.set_function("log_e", &Logger::ErrorLua);
        lua.set_function("log_w", &Logger::WarnLua);
        lua.set_function("log_i", &Logger::InfoLua);

        lua.set_exception_handler(&LuaHandler);

        lua.set_function("print", [](const std::string& message) { Logger::LogLua(message); });
    }

    void LoadAsset(sol::table assetTable, AssetManager& assetManager, SDL_Renderer* renderer, MIX_Mixer* mixer) const
    {
        if (const std::string assetType = assetTable["type"]; assetType == "texture")
        {
            assetManager.AddTexture(renderer, assetTable["id"], assetTable["file"]);
        }
        else if (assetType == "font")
        {
            const float fontSize = assetTable["font_size"];
            assetManager.AddFont(assetTable["id"], assetTable["file"], fontSize);
        }
        else if (assetType == "audio_clip")
        {
            assetManager.AddAudioClip(mixer, assetTable["id"], assetTable["file"]);
        }
        else
        {
            Logger::Error("Unknown asset type: " + assetType);
        }
    }

    [[nodiscard]] std::string GetAssetPath(const std::string& relativePath, const AssetManager& assetManager) const
    {
        return assetManager.GetFullPath(relativePath);
    }

    void SetGameMapDimensions(const double width, const double height, const Game& game)
    {
        auto& gameConfig = game.GetRegistry()->Get<GameConfig>();
        gameConfig.playableAreaHeight = static_cast<float>(height);
        gameConfig.playableAreaWidth = static_cast<float>(width);
    }
};
