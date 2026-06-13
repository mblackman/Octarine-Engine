#include "Lua/Modules/ProjectModuleLuaBinding.h"

#include <string>

#include "General/FileSandbox.h"
#include "General/Logger.h"
#include "Lua/LuaBindingContext.h"
#include "Lua/LuaTableSerializer.h"

namespace {
std::string ResolveProjectPath(LuaBindingContext& ctx, const std::string& relativePath, const char* op) {
  const std::string root = ctx.GetProjectPath();
  if (root.empty()) {
    Logger::Error(std::string("project.") + op + ": no project loaded");
    return {};
  }
  const std::string fullPath = file_sandbox::Resolve(root, relativePath);
  if (fullPath.empty()) {
    Logger::Error(std::string("project.") + op + ": path rejected: " + relativePath);
  }
  return fullPath;
}

bool WriteFile(LuaBindingContext& ctx, const std::string& relativePath, const std::string& contents) {
  const std::string fullPath = ResolveProjectPath(ctx, relativePath, "write_file");
  if (fullPath.empty()) return false;
  return file_sandbox::WriteFileAtomic(fullPath, contents);
}

bool WriteTable(LuaBindingContext& ctx, const std::string& relativePath, const sol::table& value) {
  const std::string fullPath = ResolveProjectPath(ctx, relativePath, "write_table");
  if (fullPath.empty()) return false;
  std::string text = "return ";
  std::string error;
  if (!lua_table_serializer::Serialize(value, text, &error)) {
    Logger::Error("project.write_table: " + relativePath + ": " + error);
    return false;
  }
  text += "\n";
  return file_sandbox::WriteFileAtomic(fullPath, text);
}
}  // namespace

void LuaModuleBinding<ProjectModule>::install(sol::state& lua, LuaBindingContext& ctx) {
  // Editor sessions only. A player session (even in an editor-capable binary) never gets a
  // project-folder write surface; shipped player builds exclude this module at compile time
  // (see RegisterAllModules.cpp).
  if (!ctx.IsEditorMode()) {
    return;
  }
  sol::table project = lua.create_table();
  project.set_function("path", [&ctx]() -> std::string { return ctx.GetProjectPath(); });
  project.set_function("write_file", [&ctx](const std::string& path, const std::string& contents) -> bool {
    return WriteFile(ctx, path, contents);
  });
  project.set_function("write_table", [&ctx](const std::string& path, const sol::table& value) -> bool {
    return WriteTable(ctx, path, value);
  });
  lua["project"] = project;
}
