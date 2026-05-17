#pragma once

#include <glm/glm.hpp>

#include "../Components/GlobalTransformComponent.h"
#include "../Components/PositionComponent.h"
#include "../Components/ScaleComponent.h"
#include "../Components/SpriteComponent.h"
#include "../ECS/Entity.h"
#include "../ECS/Registry.h"
#include "../Game/GameConfig.h"
#include "ECS/CommandBuffer.h"

class OffScreenDespawnSystem {
 public:
  CommandBuffer& GetCommandBuffer() { return cmd_buffer_; }

  void Prepare(Registry* registry) {
    registry_ = registry;
    const auto& gameConfig = registry_->Get<GameConfig>();

    player_ = registry_->TagId("player");

    const auto windowWidth = static_cast<float>(gameConfig.windowWidth);
    const auto windowHeight = static_cast<float>(gameConfig.windowHeight);
    playableAreaWidth_ = static_cast<float>(gameConfig.playableAreaWidth);
    playableAreaHeight_ = static_cast<float>(gameConfig.playableAreaHeight);
    if (playableAreaWidth_ <= 0) playableAreaWidth_ = windowWidth;
    if (playableAreaHeight_ <= 0) playableAreaHeight_ = windowHeight;
  }

  void operator()(const Entity entity, const PositionComponent& position, const SpriteComponent& sprite) const {
    if (registry_->HasTag(entity, player_)) return;

    const glm::vec2 globalPos = registry_->HasComponent<GlobalTransformComponent>(entity)
                                    ? registry_->GetComponent<GlobalTransformComponent>(entity).position
                                    : position.value;
    const glm::vec2 globalScale = registry_->HasComponent<GlobalTransformComponent>(entity)
                                      ? registry_->GetComponent<GlobalTransformComponent>(entity).scale
                                  : registry_->HasComponent<ScaleComponent>(entity)
                                      ? registry_->GetComponent<ScaleComponent>(entity).value
                                      : glm::vec2(1.0f, 1.0f);

    const float right = globalPos.x + sprite.width * globalScale.x;
    const float bottom = globalPos.y + sprite.height * globalScale.y;
    if (globalPos.x > playableAreaWidth_ || globalPos.y > playableAreaHeight_ || right < 0 || bottom < 0) {
      cmd_buffer_.EmplaceDespawn(entity);
    }
  }

 private:
  Registry* registry_ = nullptr;
  CommandBuffer cmd_buffer_;
  Entity player_{};
  float playableAreaWidth_ = 0.0f;
  float playableAreaHeight_ = 0.0f;
};
