#include "Lua/Modules/ProjectModuleLuaBinding.h"

#include <string>

#include "General/FileSandbox.h"
#include "General/Logger.h"
#include "Lua/LuaBindingContext.h"

namespace {
bool WriteFile(LuaBindingContext& ctx, const std::string& relativePath, const std::string& contents) {
  const std::string root = ctx.GetProjectPath();
  if (root.empty()) {
    Logger::Error("project.write_file: no project loaded");
    return false;
  }
  const std::string fullPath = file_sandbox::Resolve(root, relativePath);
  if (fullPath.empty()) {
    Logger::Error("project.write_file: path rejected: " + relativePath);
    return false;
  }
  return file_sandbox::WriteFileAtomic(fullPath, contents);
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
  lua["project"] = project;
}
