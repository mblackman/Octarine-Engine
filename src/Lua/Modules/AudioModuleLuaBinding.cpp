#include "Lua/Modules/AudioModuleLuaBinding.h"

#include <string>

#include "ECS/Registry.h"
#include "Engine/EngineContext.h"
#include "EventBus/EventBus.h"
#include "Events/AudioPlayEvent.h"
#include "Game/Game.h"
#include "General/Logger.h"

void LuaModuleBinding<AudioModule>::install(sol::state& lua, Game& game)
{
    lua.set_function("play_sound", [&game](const std::string& clipId, const sol::optional<float> volume)
    {
        auto* eventBus = game.GetContext().eventBus;
        if (!eventBus)
        {
            Logger::Error("play_sound called before event bus is ready");
            return;
        }
        eventBus->EmitEvent<AudioPlayEvent>(clipId, volume.value_or(1.0F));
    });
}
