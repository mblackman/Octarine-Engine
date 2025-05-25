#pragma once

#include "../Components/BoxColliderComponent.h"
#include "../Components/HealthComponent.h"
#include "../Components/ProjectileComponent.h"
#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "../Events/CollisionEvent.h"
#include "../General/Logger.h"

class DamageSystem : public System {
   public:
    DamageSystem() {
        RequireComponent<BoxColliderComponent>();
    }

    void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
        eventBus->SubscribeEvent<DamageSystem, CollisionEvent>(this, &DamageSystem::OnCollision);
    }

    void OnCollision(CollisionEvent& event) {
        auto a = event.entityA;
        auto b = event.entityB;
        auto aId = std::to_string(event.entityA.GetId());
        auto bId = std::to_string(event.entityB.GetId());

        if (a.InGroup("projectiles") && (b.HasTag("player") || b.InGroup("enemies"))) {
            OnProjectileHit(a, b);
        }

        if (b.InGroup("projectiles") && (a.HasTag("player") || a.InGroup("enemies"))) {
            OnProjectileHit(b, a);
        }
    }

    void OnProjectileHit(Entity projectile, Entity target) {
        auto projectileComponent = projectile.GetComponent<ProjectileComponent>();
        bool isHit = (target.HasTag("player") && !projectileComponent.isFriendly) || (target.InGroup("enemies") && projectileComponent.isFriendly);

        if (isHit) {
            auto& targetComponent = target.GetComponent<HealthComponent>();
            targetComponent.currentHealth -= projectileComponent.damage;

            if (targetComponent.currentHealth <= 0) {
                target.Blam();
            }

            projectile.Blam();
        }
    }

    void Update() {
    }
};