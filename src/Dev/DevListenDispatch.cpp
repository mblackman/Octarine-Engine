#include "Dev/DevListenDispatch.h"

#ifndef OCTARINE_SHIPPED

#include <cstdint>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <sol/sol.hpp>
#include <string>
#include <vector>

#include "AssetManager/AssetManager.h"
#include "ECS/Registry.h"
#include "Engine/EngineContext.h"
#include "Engine/SceneLoader.h"
#include "General/Logger.h"
#include "Lua/HotReload/ScriptHotReload.h"

namespace octarine::dev {
namespace {

// reply convention for the mutating ops: byte[0] = 1 ok / 0 error, remainder = utf8 message.
std::string OkReply(const std::string& message = {}) { return std::string(1, '\1') + message; }
std::string ErrReply(const std::string& message) { return std::string(1, '\0') + message; }

// Parses a push body: u32 LE path_len | path[path_len] | bytes[rest]. Returns false on truncation.
bool ParsePushBody(const std::vector<char>& body, std::string& path, const char*& bytes, std::size_t& bytes_len) {
  if (body.size() < sizeof(std::uint32_t)) return false;
  std::uint32_t path_len = 0;
  std::memcpy(&path_len, body.data(), sizeof(path_len));
  const std::size_t header = sizeof(path_len) + static_cast<std::size_t>(path_len);
  if (header > body.size()) return false;
  path.assign(body.data() + sizeof(path_len), path_len);
  bytes = body.data() + header;
  bytes_len = body.size() - header;
  return true;
}

// Writes `bytes` to <project-root>/<relPath>, creating parent dirs. Resolves the root off the
// AssetManager base path (the project dir passed on the command line). Returns the absolute path
// written, or empty on failure.
std::string WriteProjectFile(Registry& registry, const std::string& relPath, const char* bytes, std::size_t len) {
  std::string abs = registry.Get<AssetManager>().GetFullPath(relPath);
  std::error_code ec;
  std::filesystem::create_directories(std::filesystem::path(abs).parent_path(), ec);
  std::ofstream out(abs, std::ios::binary | std::ios::trunc);
  if (!out) {
    Logger::Error("DevListenServer: failed to open for write: " + abs);
    return {};
  }
  if (len > 0) out.write(bytes, static_cast<std::streamsize>(len));
  if (!out) {
    Logger::Error("DevListenServer: write failed: " + abs);
    return {};
  }
  return abs;
}

// Renders the first return value of an eval as text for the reply. Primitive types only; tables /
// userdata / functions report their type name (callers that need richer output can `return tostring(x)`
// in the snippet itself). The ok byte already signals success, so an empty string is fine here.
std::string StringifyLuaResult(const sol::protected_function_result& r) {
  if (r.return_count() < 1) return {};
  const sol::object first = r.get<sol::object>(0);
  switch (first.get_type()) {
    case sol::type::string:
      return first.as<std::string>();
    case sol::type::number:
      return std::to_string(first.as<double>());
    case sol::type::boolean:
      return first.as<bool>() ? "true" : "false";
    case sol::type::nil:
    case sol::type::none:
      return {};
    default:
      return "<" + std::string(sol::type_name(first.lua_state(), first.get_type())) + ">";
  }
}

void HandleEvalLua(sol::state& lua, const std::vector<char>& body, std::string& reply_body) {
  const std::string src(body.data(), body.size());
  const sol::protected_function_result r = lua.safe_script(src, sol::script_pass_on_error);
  if (r.valid()) {
    reply_body = OkReply(StringifyLuaResult(r));
  } else {
    const sol::error err = r;
    reply_body = ErrReply(err.what());
  }
}

void HandleReloadScene(Registry& registry, std::string& reply_body) {
  auto* loader = registry.TryGet<SceneLoader*>();
  if (loader == nullptr || *loader == nullptr) {
    reply_body = ErrReply("no scene loader");
    return;
  }
  (*loader)->ReloadScene();
  reply_body = OkReply();
}

void HandlePushScript(Registry& registry, sol::state& lua, const std::vector<char>& body, std::string& reply_body) {
  std::string path;
  const char* bytes = nullptr;
  std::size_t len = 0;
  if (!ParsePushBody(body, path, bytes, len)) {
    reply_body = ErrReply("malformed push body");
    return;
  }
  if (WriteProjectFile(registry, path, bytes, len).empty()) {
    reply_body = ErrReply("write failed: " + path);
    return;
  }
  // Force a reload so the new source is live this frame rather than waiting on the mtime poll.
  if (auto* hot = registry.TryGet<ScriptHotReload>()) {
    hot->ForceReloadAll(registry, lua);
  }
  reply_body = OkReply(path);
}

void HandlePushAsset(Registry& registry, const std::vector<char>& body, std::string& reply_body) {
  std::string path;
  const char* bytes = nullptr;
  std::size_t len = 0;
  if (!ParsePushBody(body, path, bytes, len)) {
    reply_body = ErrReply("malformed push body");
    return;
  }
  const std::string abs = WriteProjectFile(registry, path, bytes, len);
  if (abs.empty()) {
    reply_body = ErrReply("write failed: " + path);
    return;
  }
  auto& engine = registry.Get<EngineContext>();
  registry.Get<AssetManager>().ReloadByPath(abs, engine.sdlRenderer, engine.mixer);
  reply_body = OkReply(path);
}

}  // namespace

CommandHandler MakeEngineCommandHandler(Registry& registry, sol::state& lua) {
  Registry* reg = &registry;
  sol::state* lua_ptr = &lua;
  return [reg, lua_ptr](OpCode op, const std::vector<char>& body, std::uint32_t& reply_op, std::string& reply_body) {
    reply_op = static_cast<std::uint32_t>(op);
    switch (op) {
      case OpCode::EvalLua:
        HandleEvalLua(*lua_ptr, body, reply_body);
        return;
      case OpCode::ReloadScene:
        HandleReloadScene(*reg, reply_body);
        return;
      case OpCode::PushScript:
        HandlePushScript(*reg, *lua_ptr, body, reply_body);
        return;
      case OpCode::PushAsset:
        HandlePushAsset(*reg, body, reply_body);
        return;
      default:
        reply_op = kErrorReplyOp;
        reply_body.clear();
        return;
    }
  };
}

}  // namespace octarine::dev

#endif  // !OCTARINE_SHIPPED
