#include "Lua/Bindings/RegisterAllBindings.h"

#include "Lua/Bindings/AnimationComponentLuaBinding.h"
#include "Lua/Bindings/AudioListenerComponentLuaBinding.h"
#include "Lua/Bindings/AudioSourceComponentLuaBinding.h"
#include "Lua/Bindings/BoxColliderComponentLuaBinding.h"
#include "Lua/Bindings/CameraFollowComponentLuaBinding.h"
#include "Lua/Bindings/HealthComponentLuaBinding.h"
#include "Lua/Bindings/LuaComponentRegistry.h"
#include "Lua/Bindings/NameComponentLuaBinding.h"
#include "Lua/Bindings/PositionComponentLuaBinding.h"
#include "Lua/Bindings/ProjectileEmitterComponentLuaBinding.h"
#include "Lua/Bindings/RigidBodyComponentLuaBinding.h"
#include "Lua/Bindings/RotationComponentLuaBinding.h"
#include "Lua/Bindings/ScaleComponentLuaBinding.h"
#include "Lua/Bindings/ScriptComponentLuaBinding.h"
#include "Lua/Bindings/SpriteComponentLuaBinding.h"
#include "Lua/Bindings/SquarePrimitiveComponentLuaBinding.h"
#include "Lua/Bindings/TextLabelComponentLuaBinding.h"
#include "Lua/Bindings/UIAnchorComponentLuaBinding.h"
#include "Lua/Bindings/UIButtonComponentLuaBinding.h"
#include "Lua/Bindings/UICanvasComponentLuaBinding.h"
#include "Lua/Bindings/UIRectComponentLuaBinding.h"

void RegisterAllLuaBindings() {
  LuaComponentRegistry::clear();
  LuaComponentRegistry::registerComponent<RigidBodyComponent>();
  LuaComponentRegistry::registerComponent<SpriteComponent>();
  LuaComponentRegistry::registerComponent<SquarePrimitiveComponent>();
  LuaComponentRegistry::registerComponent<AnimationComponent>();
  LuaComponentRegistry::registerComponent<BoxColliderComponent>();
  LuaComponentRegistry::registerComponent<HealthComponent>();
  LuaComponentRegistry::registerComponent<ProjectileEmitterComponent>();
  LuaComponentRegistry::registerComponent<CameraFollowComponent>();
  LuaComponentRegistry::registerComponent<ScriptComponent>();
  LuaComponentRegistry::registerComponent<UIButtonComponent>();
  LuaComponentRegistry::registerComponent<TextLabelComponent>();
  LuaComponentRegistry::registerComponent<AudioSourceComponent>();
  LuaComponentRegistry::registerComponent<AudioListenerComponent>();
  LuaComponentRegistry::registerComponent<NameComponent>();
  LuaComponentRegistry::registerComponent<ScaleComponent>();
  LuaComponentRegistry::registerComponent<PositionComponent>();
  LuaComponentRegistry::registerComponent<RotationComponent>();
  LuaComponentRegistry::registerComponent<UICanvasComponent>();
  LuaComponentRegistry::registerComponent<UIAnchorComponent>();
  LuaComponentRegistry::registerComponent<UIRectComponent>();
}
