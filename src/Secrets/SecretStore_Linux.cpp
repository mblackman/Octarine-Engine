#include "Secrets/SecretStore.h"

// Built for everything that is neither Windows nor Apple — primarily Linux desktop, and Android
// (where the editor isn't compiled anyway, but the stubs harm nothing). Returns "not available"
// so the editor surfaces the env-var contract instead of offering a save UI.

#if !defined(_WIN32) && !defined(__APPLE__)

namespace octarine::secrets {
bool IsAvailable() { return false; }

bool Set(std::string_view /*key*/, std::string_view /*value*/) { return false; }

std::optional<std::string> Get(std::string_view /*key*/) { return std::nullopt; }

bool Clear(std::string_view /*key*/) { return false; }
}  // namespace octarine::secrets

#endif
