#include "ECSLoader.h"

#include <sol/sol.hpp>

#include "../Components/AnimationComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/CameraFollowComponent.h"
#include "../Components/HealthComponent.h"
#include "../Components/KeyboardControlComponent.h"
#include "../Components/ProjectileEmitterComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/ScriptComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/SquarePrimitiveComponent.h"
#include "../Components/TextLabelComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/UIButtonComponent.h"

void ECSLoader::LoadEntity(sol::table entityTable, const std::unique_ptr<Registry>& registry) {
    Entity newEntity = registry->CreateEntity();

    // Tag
    sol::optional<std::string> tag = entityTable["tag"];
    if (tag != sol::nullopt) {
        newEntity.Tag(entityTable["tag"]);
    }

    // Group
    sol::optional<std::string> group = entityTable["group"];
    if (group != sol::nullopt) {
        newEntity.Group(entityTable["group"]);
    }

    // Components
    sol::optional<sol::table> hasComponents = entityTable["components"];
    if (hasComponents != sol::nullopt) {
        // Transform
        sol::optional<sol::table> transform = entityTable["components"]["transform"];
        if (transform != sol::nullopt) {
            newEntity.AddComponent<TransformComponent>(
                glm::vec2(
                    entityTable["components"]["transform"]["position"]["x"],
                    entityTable["components"]["transform"]["position"]["y"]),
                glm::vec2(
                    entityTable["components"]["transform"]["scale"]["x"].get_or(1.0),
                    entityTable["components"]["transform"]["scale"]["y"].get_or(1.0)),
                entityTable["components"]["transform"]["rotation"].get_or(0.0));
        }

        // RigidBody
        sol::optional<sol::table> rigidbody = entityTable["components"]["rigidbody"];
        if (rigidbody != sol::nullopt) {
            newEntity.AddComponent<RigidBodyComponent>(
                glm::vec2(
                    entityTable["components"]["rigidbody"]["velocity"]["x"].get_or(0.0),
                    entityTable["components"]["rigidbody"]["velocity"]["y"].get_or(0.0)));
        }

        // Sprite
        sol::optional<sol::table> sprite = entityTable["components"]["sprite"];
        if (sprite != sol::nullopt) {
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
        sol::optional<sol::table> square = entityTable["components"]["square"];
        if (square != sol::nullopt) {
            sol::table color = entityTable["components"]["square"]["color"];
            Uint8 alphaDefault = 255;
            SDL_Color sdlColor = {color["r"], color["g"], color["b"], color["a"].get_or(alphaDefault)};
            newEntity.AddComponent<SquarePrimitiveComponent>(
                glm::vec2(
                    entityTable["components"]["square"]["position"]["x"].get_or(0),
                    entityTable["components"]["square"]["position"]["y"].get_or(0)),
                entityTable["components"]["square"]["layer"].get_or(1),
                entityTable["components"]["square"]["width"],
                entityTable["components"]["square"]["height"],
                sdlColor,
                entityTable["components"]["square"]["is_fixed"].get_or(false));
        }

        // Animation
        sol::optional<sol::table> animation = entityTable["components"]["animation"];
        if (animation != sol::nullopt) {
            newEntity.AddComponent<AnimationComponent>(
                entityTable["components"]["animation"]["num_frames"].get_or(1),
                entityTable["components"]["animation"]["speed_rate"].get_or(1));
        }

        // BoxCollider
        sol::optional<sol::table> collider = entityTable["components"]["boxcollider"];
        if (collider != sol::nullopt) {
            newEntity.AddComponent<BoxColliderComponent>(
                entityTable["components"]["boxcollider"]["width"],
                entityTable["components"]["boxcollider"]["height"],
                glm::vec2(
                    entityTable["components"]["boxcollider"]["offset"]["x"].get_or(0),
                    entityTable["components"]["boxcollider"]["offset"]["y"].get_or(0)));
        }

        // Health
        sol::optional<sol::table> health = entityTable["components"]["health"];
        if (health != sol::nullopt) {
            newEntity.AddComponent<HealthComponent>(
                static_cast<int>(entityTable["components"]["health"]["max_health"].get_or(100)));
        }

        // ProjectileEmitter
        sol::optional<sol::table> projectileEmitter = entityTable["components"]["projectile_emitter"];
        if (projectileEmitter != sol::nullopt) {
            newEntity.AddComponent<ProjectileEmitterComponent>(
                glm::vec2(
                    entityTable["components"]["projectile_emitter"]["projectile_velocity"]["x"],
                    entityTable["components"]["projectile_emitter"]["projectile_velocity"]["y"]),
                static_cast<int>(entityTable["components"]["projectile_emitter"]["repeat_frequency"].get_or(1)) * 1000,
                static_cast<int>(entityTable["components"]["projectile_emitter"]["projectile_duration"].get_or(10)) * 1000,
                static_cast<int>(entityTable["components"]["projectile_emitter"]["hit_damage"].get_or(10)),
                entityTable["components"]["projectile_emitter"]["friendly"].get_or(false));
        }

        // CameraFollow
        sol::optional<sol::table> cameraFollow = entityTable["components"]["camera_follow"];
        if (cameraFollow != sol::nullopt) {
            newEntity.AddComponent<CameraFollowComponent>();
        }

        // KeyboardControlled
        sol::optional<sol::table> keyboardControlled = entityTable["components"]["keyboard_controller"];
        if (keyboardControlled != sol::nullopt) {
            double velocity = entityTable["components"]["keyboard_controller"]["velocity"];
            newEntity.AddComponent<KeyboardControlComponent>(velocity);
        }

        sol::optional<sol::table> onUpdateScript = entityTable["components"]["on_update_script"];
        if (onUpdateScript != sol::nullopt) {
            sol::function scriptFunction = entityTable["components"]["on_update_script"][0];
            newEntity.AddComponent<ScriptComponent>(scriptFunction);
        }

        sol::optional<sol::table> button = entityTable["components"]["button"];
        if (button != sol::nullopt) {
            sol::table buttonTable = entityTable["components"]["button"];
            bool isActive = entityTable["components"]["button"]["is_active"].get_or(false);
            sol::optional<sol::table> onClick = entityTable["components"]["button"]["on_click_script"];

            if (onClick != sol::nullopt) {
                sol::function clickFunction = entityTable["components"]["button"]["on_click_script"][0];
                newEntity.AddComponent<UIButtonComponent>(isActive, buttonTable, clickFunction);
            } else {
                newEntity.AddComponent<UIButtonComponent>(isActive, buttonTable);
            }
        }
    }
}

void ECSLoader::LoadAsset(sol::table assetTable, const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer) {
    std::string assetType = assetTable["type"];

    if (assetType == "texture") {
        assetManager->AddTexture(renderer, assetTable["id"], assetTable["file"]);
    } else if (assetType == "font") {
        int fontSize = assetTable["font_size"];
        assetManager->AddFont(assetTable["id"], assetTable["file"], fontSize);
    } else {
        Logger::Error("Unknown asset type: " + assetType);
    }
}