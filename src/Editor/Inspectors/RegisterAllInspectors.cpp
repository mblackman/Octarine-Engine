#include "Editor/Inspectors/RegisterAllInspectors.h"

#ifdef OCTARINE_WITH_EDITOR

#include "Editor/Inspectors/AnimationComponentInspector.h"
#include "Editor/Inspectors/BoxColliderComponentInspector.h"
#include "Editor/Inspectors/CameraFollowComponentInspector.h"
#include "Editor/Inspectors/ComponentInspectorRegistry.h"
#include "Editor/Inspectors/EntityMaskComponentInspector.h"
#include "Editor/Inspectors/GlobalTransformComponentInspector.h"
#include "Editor/Inspectors/HealthComponentInspector.h"
#include "Editor/Inspectors/PositionComponentInspector.h"
#include "Editor/Inspectors/ProjectileComponentInspector.h"
#include "Editor/Inspectors/ProjectileEmitterComponentInspector.h"
#include "Editor/Inspectors/RigidBodyComponentInspector.h"
#include "Editor/Inspectors/RotationComponentInspector.h"
#include "Editor/Inspectors/ScaleComponentInspector.h"
#include "Editor/Inspectors/ScriptComponentInspector.h"
#include "Editor/Inspectors/SpriteComponentInspector.h"
#include "Editor/Inspectors/SquarePrimitiveComponentInspector.h"
#include "Editor/Inspectors/TextLabelComponentInspector.h"
#include "Editor/Inspectors/UIButtonComponentInspector.h"

void RegisterAllComponentInspectors() {
  ComponentInspectorRegistry::clear();
  // Order = display order in the inspector.
  ComponentInspectorRegistry::registerComponent<PositionComponent>();
  ComponentInspectorRegistry::registerComponent<ScaleComponent>();
  ComponentInspectorRegistry::registerComponent<RotationComponent>();
  ComponentInspectorRegistry::registerComponent<GlobalTransformComponent>();
  ComponentInspectorRegistry::registerComponent<HealthComponent>();
  ComponentInspectorRegistry::registerComponent<SpriteComponent>();
  ComponentInspectorRegistry::registerComponent<RigidBodyComponent>();
  ComponentInspectorRegistry::registerComponent<BoxColliderComponent>();
  ComponentInspectorRegistry::registerComponent<AnimationComponent>();
  ComponentInspectorRegistry::registerComponent<ProjectileEmitterComponent>();
  ComponentInspectorRegistry::registerComponent<TextLabelComponent>();
  ComponentInspectorRegistry::registerComponent<ScriptComponent>();
  ComponentInspectorRegistry::registerComponent<CameraFollowComponent>();
  ComponentInspectorRegistry::registerComponent<EntityMaskComponent>();
  ComponentInspectorRegistry::registerComponent<ProjectileComponent>();
  ComponentInspectorRegistry::registerComponent<SquarePrimitiveComponent>();
  ComponentInspectorRegistry::registerComponent<UIButtonComponent>();
}

#endif  // OCTARINE_WITH_EDITOR
