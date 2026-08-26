#pragma once

#include <glm/glm.hpp>
#include <memory>

#include "Components/BoxColliderComponent.h"
#include "Components/RotationComponent.h"
#include "Components/SpriteComponent.h"
#include "Components/SquarePrimitiveComponent.h"
#include "ECS/Iterable.h"
#include "ECS/Query.h"
#include "ECS/Registry.h"
#include "General/PerfUtils.h"

// Bakes each entity's normalized pivot into local pixel units ahead of TransformSystem.
//
// A normalized anchor is ergonomic to author but needs the entity's dimensions to resolve, and
// those live in a different component than the transform. Resolving it inside the propagation
// loop would make every entity's pivot cost a second archetype lookup on the hottest path in the
// frame. Doing it once here, into RotationComponent::pivotOffset, keeps the propagation loop
// reading nothing but transform data.
//
// Size precedence is sprite, then primitive, then collider: an entity carrying more than one is
// anchored to the thing it actually draws. With no size source the offset stays zero, which
// rotates about `position` — the same result a (0, 0) pivot gives.
class PivotResolveSystem {
 public:
  void operator()(const ContextFacade& ctx, const Iterable& /*iter*/) {
    PROFILE_NAMED_SCOPE("PivotResolveSystem");
    auto* registry = ctx.GetRegistry();
    if (!query_) {
      query_ = registry->CreateQuery<RotationComponent, Opt<SpriteComponent>, Opt<SquarePrimitiveComponent>,
                                     Opt<BoxColliderComponent>>();
    }
    query_->Update();

    query_->ParallelForEach(
        [](RotationComponent& rotation, const SpriteComponent* sprite, const SquarePrimitiveComponent* square,
           const BoxColliderComponent* collider) {
          rotation.pivotOffset = rotation.pivot * LocalSize(sprite, square, collider);
        },
        kSerialBelowEntities);
  }

 private:
  // Mirrors TransformSystem's flat-pass threshold: the per-entity body is a multiply and a
  // branch, so thread-pool dispatch only earns its fixed cost on large worlds.
  static constexpr size_t kSerialBelowEntities = 8192;

  static glm::vec2 LocalSize(const SpriteComponent* sprite, const SquarePrimitiveComponent* square,
                             const BoxColliderComponent* collider) {
    if (sprite) return {sprite->width, sprite->height};
    if (square) return {square->width, square->height};
    if (collider) return {static_cast<float>(collider->width), static_cast<float>(collider->height)};
    return {0.0f, 0.0f};
  }

  using PivotQuery =
      ComponentQuery<RotationComponent, Opt<SpriteComponent>, Opt<SquarePrimitiveComponent>, Opt<BoxColliderComponent>>;

  std::unique_ptr<PivotQuery> query_;
};
