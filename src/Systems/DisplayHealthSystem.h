#pragma once

#include <SDL2/SDL.h>

#include "../Components/HealthComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/SquarePrimitiveComponent.h"
#include "../Components/TextLabelComponent.h"
#include "../Components/TransformComponent.h"
#include "../ECS/ECS.h"

class DisplayHealthSystem : public System {
   private:
    std::unordered_map<int, std::shared_ptr<Entity>> health_trackers_;
    std::set<int> living_entities_;
    std::set<int> removed_entities_;
    SDL_Color low_health_color = {255, 0, 0};
    SDL_Color medium_health_color = {255, 255, 0};
    SDL_Color high_health_color = {0, 255, 0};

   public:
    DisplayHealthSystem() : health_trackers_(), living_entities_(), removed_entities_() {
        RequireComponent<HealthComponent>();
        RequireComponent<TransformComponent>();
    }

    ~DisplayHealthSystem() = default;

    void Update(std::unique_ptr<Registry>& registry) {
        living_entities_.clear();
        removed_entities_.clear();

        for (auto entity : GetEntities()) {
            living_entities_.emplace(entity.GetId());

            if (health_trackers_.find(entity.GetId()) == health_trackers_.end()) {
                CreateHealthTracker(registry, entity.GetId());
            }

            auto transform = entity.GetComponent<TransformComponent>();
            auto health = entity.GetComponent<HealthComponent>();
            auto& textLabel = health_trackers_[entity.GetId()]->GetComponent<TextLabelComponent>();
            auto& square = health_trackers_[entity.GetId()]->GetComponent<SquarePrimitiveComponent>();

            float healthPercentage = static_cast<float>(health.currentHealth) / health.maxHealth;
            int healthAmount = static_cast<int>(healthPercentage * 100);
            textLabel.text = std::to_string(healthAmount) + "%";
            textLabel.position = glm::vec2(transform.position.x, transform.position.y - 25);
            textLabel.color = GetHealthColor(healthPercentage);

            int healthWidth = healthAmount;

            if (entity.HasComponent<SpriteComponent>()) {
                auto sprite = entity.GetComponent<SpriteComponent>();
                healthWidth = static_cast<int>(sprite.width * healthPercentage * transform.scale.x);
            }

            square.position = glm::vec2(transform.position.x, transform.position.y - 5);
            square.width = healthWidth;
            square.color = GetHealthColor(healthPercentage);
        }

        for (auto healthTracker : health_trackers_) {
            if (living_entities_.find(healthTracker.first) == living_entities_.end()) {
                healthTracker.second->Blam();
                living_entities_.erase(healthTracker.first);
                removed_entities_.emplace(healthTracker.first);
            }
        }

        for (auto entityId : removed_entities_) {
            health_trackers_.erase(entityId);
        }
    }

   private:
    void CreateHealthTracker(std::unique_ptr<Registry>& registry, int entityId) {
        auto healthTracker = registry->CreateEntity();
        healthTracker.AddComponent<TextLabelComponent>(glm::vec2(0, 0), 100, "100", "arial-font-10", SDL_Color{255, 255, 255}, false);
        healthTracker.AddComponent<SquarePrimitiveComponent>(glm::vec2(0, 0), 100, 100, 10, SDL_Color{255, 0, 0}, false);
        health_trackers_[entityId] = std::make_shared<Entity>(healthTracker);
    }

    SDL_Color GetHealthColor(float healthPercentage) {
        if (healthPercentage > 0.66) {
            return high_health_color;
        } else if (healthPercentage > 0.33) {
            return medium_health_color;
        } else {
            return low_health_color;
        }
    }
};