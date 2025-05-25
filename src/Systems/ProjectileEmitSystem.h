#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_keycode.h>

#include <glm/glm.hpp>

#include "../Components/BoxColliderComponent.h"
#include "../Components/ProjectileComponent.h"
#include "../Components/ProjectileEmitterComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"
#include "../ECS/ECS.h"
#include "../General/Logger.h"

class ProjectileEmitSystem : public System {
   public:
    ProjectileEmitSystem() : spawnFriendlyProjectiles_(false) {
        RequireComponent<TransformComponent>();
        RequireComponent<ProjectileEmitterComponent>();
    }

    ~ProjectileEmitSystem() = default;

    void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
        eventBus->SubscribeEvent<ProjectileEmitSystem, KeyInputEvent>(this, &ProjectileEmitSystem::OnKeyInput);
    }

    void OnKeyInput(KeyInputEvent& event) {
        if (!event.isPressed) {
            return;
        }

        switch (event.inputKey) {
            case SDLK_SPACE:
                spawnFriendlyProjectiles_ = true;
                break;
        }
    }

    void Update(std::unique_ptr<Registry>& registry) {
        for (auto entity : GetEntities()) {
            auto transform = entity.GetComponent<TransformComponent>();
            auto& emitter = entity.GetComponent<ProjectileEmitterComponent>();

            if (!emitter.isFriendly && static_cast<int>(SDL_GetTicks()) - emitter.lastEmissionTime > emitter.frequency) {
                SpawnProjectile(transform, entity, registry, emitter);
            } else if (emitter.isFriendly && spawnFriendlyProjectiles_) {
                SpawnProjectile(transform, entity, registry, emitter);
                spawnFriendlyProjectiles_ = false;
            }
        }
    }

   private:
    bool spawnFriendlyProjectiles_;

    void SpawnProjectile(TransformComponent& transform, Entity& entity, std::unique_ptr<Registry>& registry, ProjectileEmitterComponent& emitter) {
        auto projectilePosition = transform.position;
        auto velocity = emitter.velocity;

        if (entity.HasComponent<SpriteComponent>()) {
            auto sprite = entity.GetComponent<SpriteComponent>();
            projectilePosition.x += (transform.scale.x * sprite.width / 2);
            projectilePosition.y += (transform.scale.y * sprite.height / 2);
        }

        if (emitter.isFriendly && entity.HasComponent<RigidBodyComponent>()) {
            auto rigidBody = entity.GetComponent<RigidBodyComponent>();
            auto direction = glm::normalize(rigidBody.velocity);
            velocity = direction * emitter.velocity;
        }

        auto projectile = registry->CreateEntity();
        projectile.Group("projectiles");
        projectile.AddComponent<TransformComponent>(projectilePosition, glm::vec2(1.0, 1.0), 0.0);
        projectile.AddComponent<RigidBodyComponent>(velocity);
        projectile.AddComponent<BoxColliderComponent>(4, 4);
        projectile.AddComponent<SpriteComponent>("bullet-texture", 4, 4, 4);
        projectile.AddComponent<ProjectileComponent>(emitter.damage, SDL_GetTicks(), emitter.duration, emitter.isFriendly);

        emitter.lastEmissionTime = SDL_GetTicks();
    }
};