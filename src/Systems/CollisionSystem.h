#pragma once

#include "../Components/BoxColliderComponent.h"
#include "../Components/TransformComponent.h"
#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "../Events/CollisionEvent.h"
#include "../General/Logger.h"

class CollisionSystem : public System {
   public:
    CollisionSystem() {
        RequireComponent<TransformComponent>();
        RequireComponent<BoxColliderComponent>();
    }

    ~CollisionSystem() = default;

    void Update(std::unique_ptr<EventBus>& eventBus) {
        auto entities = GetEntities();

        for (auto i = entities.begin(); i != entities.end(); i++) {
            auto entityA = *i;
            const auto& transformA = entityA.GetComponent<TransformComponent>();
            const auto& colliderA = entityA.GetComponent<BoxColliderComponent>();

            for (auto j = i + 1; j != entities.end(); j++) {
                auto entityB = *j;
                const auto& transformB = entityB.GetComponent<TransformComponent>();
                const auto& colliderB = entityB.GetComponent<BoxColliderComponent>();
                bool isColliding = CheckAABBCollision(transformA, colliderA, transformB, colliderB);
                if (isColliding) {
                    eventBus->EmitEvent<CollisionEvent>(entityA, entityB);
                }
            }
        }
    }

    bool CheckAABBCollision(const TransformComponent& transformA, const BoxColliderComponent& colliderA, const TransformComponent& transformB, const BoxColliderComponent& colliderB) {
        return !(
            (transformA.position.x > (transformB.position.x + colliderB.width * transformB.scale.x)) || ((transformA.position.x + colliderA.width * transformA.scale.x) < transformB.position.x) || ((transformA.position.y + colliderA.height * transformA.scale.y) < transformB.position.y) || (transformA.position.y > (transformB.position.y + colliderB.height * transformB.scale.y)));
    }
};