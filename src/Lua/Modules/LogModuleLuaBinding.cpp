#include "Lua/Modules/LogModuleLuaBinding.h"

#include <exception>
#include <string>

#include "General/Logger.h"

namespace {
int LuaHandler(lua_State* /*lua_state*/, sol::optional<const std::exception&> exception,
               const sol::string_view string_view) {
  if (exception) {
    const std::exception& err = exception.value();
    Logger::ErrorLua(std::string(err.what()));
  } else if (!string_view.empty()) {
    Logger::ErrorLua(std::string(string_view.data(), string_view.size()));
  } else {
    Logger::ErrorLua("Unknown Lua Error!");
  }
  return 0;
}
}  // namespace

void LuaModuleBinding<LogModule>::install(sol::state& lua, Game& /*game*/) {
  lua.set_function("log", &Logger::LogLua);
  lua.set_function("log_e", &Logger::ErrorLua);
  lua.set_function("log_w", &Logger::WarnLua);
  lua.set_function("log_i", &Logger::InfoLua);
  lua.set_function("print", [](const std::string& message) { Logger::LogLua(message); });

  lua.set_exception_handler(&LuaHandler);
}
