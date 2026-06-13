#include "Lua/Modules/StorageModuleLuaBinding.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <system_error>
#include <vector>

#include "General/FileSandbox.h"
#include "General/Logger.h"
#include "Lua/LuaBindingContext.h"
#include "Lua/LuaTableSerializer.h"

namespace {
// Resolve a script-supplied relative path inside the per-user save-data root. Empty result
// means rejected (absolute path, ../ escape, or no save root available) — already logged.
std::string ResolveStoragePath(LuaBindingContext& ctx, const std::string& relativePath, const char* op) {
  const std::string root = ctx.GetSaveDataPath();
  if (root.empty()) {
    Logger::Error(std::string("storage.") + op + ": no save-data root available");
    return {};
  }
  std::string fullPath = file_sandbox::Resolve(root, relativePath);
  if (fullPath.empty()) {
    Logger::Error(std::string("storage.") + op + ": path rejected: " + relativePath);
  }
  return fullPath;
}

bool Write(LuaBindingContext& ctx, const std::string& relativePath, const std::string& contents) {
  const std::string fullPath = ResolveStoragePath(ctx, relativePath, "write");
  if (fullPath.empty()) return false;
  return file_sandbox::WriteFileAtomic(fullPath, contents);
}

sol::optional<std::string> Read(LuaBindingContext& ctx, const std::string& relativePath) {
  const std::string fullPath = ResolveStoragePath(ctx, relativePath, "read");
  if (fullPath.empty()) return sol::nullopt;
  std::ifstream file(fullPath, std::ios::binary);
  if (!file.is_open()) return sol::nullopt;
  std::ostringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}

bool Exists(LuaBindingContext& ctx, const std::string& relativePath) {
  const std::string fullPath = ResolveStoragePath(ctx, relativePath, "exists");
  if (fullPath.empty()) return false;
  std::error_code ec;
  return std::filesystem::exists(fullPath, ec);
}

bool Remove(LuaBindingContext& ctx, const std::string& relativePath) {
  const std::string fullPath = ResolveStoragePath(ctx, relativePath, "remove");
  if (fullPath.empty()) return false;
  std::error_code ec;
  return std::filesystem::remove(fullPath, ec);
}

bool WriteTable(LuaBindingContext& ctx, const std::string& relativePath, const sol::table& value) {
  const std::string fullPath = ResolveStoragePath(ctx, relativePath, "write_table");
  if (fullPath.empty()) return false;
  std::string text = "return ";
  std::string error;
  if (!lua_table_serializer::Serialize(value, text, &error)) {
    Logger::Error("storage.write_table: " + relativePath + ": " + error);
    return false;
  }
  text += "\n";
  return file_sandbox::WriteFileAtomic(fullPath, text);
}

sol::optional<sol::table> ReadTable(LuaBindingContext& ctx, const std::string& relativePath, sol::this_state s) {
  const auto text = Read(ctx, relativePath);
  if (!text) return sol::nullopt;
  sol::state_view lua(s);
  sol::load_result chunk = lua.load(*text, "@" + relativePath, sol::load_mode::text);
  if (!chunk.valid()) {
    const sol::error err = chunk;
    Logger::Error("storage.read_table: " + relativePath + ": " + std::string(err.what()));
    return sol::nullopt;
  }
  auto fn = chunk.get<sol::protected_function>();
  // Empty environment: a save file is user-editable data, not code. The chunk sees no globals
  // at all (no os/io/load/print), so a tampered file can only compute and return a value.
  sol::environment env(lua, sol::create);
  sol::set_environment(env, fn);
  sol::protected_function_result result = fn();
  if (!result.valid()) {
    const sol::error err = result;
    Logger::Error("storage.read_table: " + relativePath + ": " + std::string(err.what()));
    return sol::nullopt;
  }
  const auto value = result.get<sol::object>();
  if (value.get_type() != sol::type::table) {
    Logger::Error("storage.read_table: " + relativePath + ": file did not return a table");
    return sol::nullopt;
  }
  return value.as<sol::table>();
}

// Names of the direct children of `relativePath` (files and subdirectories), sorted for stable
// iteration order. Empty table when the directory doesn't exist.
sol::table List(LuaBindingContext& ctx, const std::string& relativePath, sol::this_state s) {
  sol::state_view lua(s);
  sol::table names = lua.create_table();
  const std::string fullPath = ResolveStoragePath(ctx, relativePath, "list");
  if (fullPath.empty()) return names;

  std::error_code ec;
  std::vector<std::string> entries;
  for (const auto& entry : std::filesystem::directory_iterator(fullPath, ec)) {
    entries.push_back(entry.path().filename().string());
  }
  std::sort(entries.begin(), entries.end());
  for (size_t i = 0; i < entries.size(); ++i) {
    names[i + 1] = entries[i];
  }
  return names;
}
}  // namespace

void LuaModuleBinding<StorageModule>::install(sol::state& lua, LuaBindingContext& ctx) {
  sol::table storage = lua.create_table();
  storage.set_function("write", [&ctx](const std::string& path, const std::string& contents) -> bool {
    return Write(ctx, path, contents);
  });
  storage.set_function("read",
                       [&ctx](const std::string& path) -> sol::optional<std::string> { return Read(ctx, path); });
  storage.set_function("write_table", [&ctx](const std::string& path, const sol::table& value) -> bool {
    return WriteTable(ctx, path, value);
  });
  storage.set_function("read_table", [&ctx](const std::string& path, sol::this_state s) -> sol::optional<sol::table> {
    return ReadTable(ctx, path, s);
  });
  storage.set_function("exists", [&ctx](const std::string& path) -> bool { return Exists(ctx, path); });
  storage.set_function("remove", [&ctx](const std::string& path) -> bool { return Remove(ctx, path); });
  // Path optional: storage.list() lists the save-data root itself.
  storage.set_function("list", [&ctx](const sol::optional<std::string>& path, sol::this_state s) -> sol::table {
    return List(ctx, path.value_or(""), s);
  });
  lua["storage"] = storage;
}
