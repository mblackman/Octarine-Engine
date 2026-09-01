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

void LuaModuleBinding<LogModule>::install(sol::state& lua, LuaBindingContext& /*ctx*/) {
  lua.set_function("log", &Logger::LogLua);
  lua.set_function("log_e", &Logger::ErrorLua);
  lua.set_function("log_w", &Logger::WarnLua);
  lua.set_function("log_i", &Logger::InfoLua);
  lua.set_function("print", [](sol::variadic_args va) {
    std::string msg;
    bool first = true;
    for (auto v : va) {
      if (!first) msg += "\t";
      first = false;
      if (v.is<std::string>()) {
        msg += v.as<std::string>();
      } else {
        sol::state_view sv(v.lua_state());
        msg += sv["tostring"](v).get<std::string>();
      }
    }
    Logger::LogLua(msg);
  });

  lua.set_exception_handler(&LuaHandler);
}
