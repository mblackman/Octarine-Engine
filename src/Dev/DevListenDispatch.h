#pragma once

#ifndef OCTARINE_SHIPPED

#include "Dev/DevListenServer.h"

class Registry;
namespace sol {
class state;
}

namespace octarine::dev {

// Builds the main-thread CommandHandler that applies the dev-listen engine-mutating ops (eval_lua,
// reload_scene, push_script, push_asset) against the live engine. Captures non-owning references to
// the Registry + sol::state, both stable for the engine's lifetime. Install the result via
// DevListenServer::SetCommandHandler. Lives in the engine layer so the transport (DevListenServer)
// stays free of Registry / sol / AssetManager dependencies and its smoke test can build standalone.
CommandHandler MakeEngineCommandHandler(Registry& registry, sol::state& lua);

}  // namespace octarine::dev

#endif  // !OCTARINE_SHIPPED
