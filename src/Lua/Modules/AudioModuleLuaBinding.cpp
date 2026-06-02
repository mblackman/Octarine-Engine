#include "Lua/Modules/AudioModuleLuaBinding.h"

#include <string>

#include "ECS/Registry.h"
#include "Engine/EngineContext.h"
#include "EventBus/EventBus.h"
#include "Events/AudioPlayEvent.h"
#include "General/Logger.h"
#include "Lua/LuaBindingContext.h"

void LuaModuleBinding<AudioModule>::install(sol::state& lua, LuaBindingContext& ctx) {
  lua.set_function("play_sound", [&ctx](const std::string& clipId, const sol::optional<float> volume) {
    auto* eventBus = ctx.GetContext().eventBus;
    if (!eventBus) {
      Logger::Error("play_sound called before event bus is ready");
      return;
    }
    eventBus->EmitEvent<AudioPlayEvent>(clipId, volume.value_or(1.0F));
  });
}
