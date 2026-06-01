#include "Lua/Modules/IoModuleLuaBinding.h"

#include <SDL3/SDL.h>

#include <sstream>
#include <string>
#include <vector>

#include "General/Logger.h"

namespace {
// Read via SDL IO (SDL_LoadFile wraps SDL_IOFromFile) so paths resolve on desktop, an APK asset
// root, or a .app bundle alike.
std::vector<std::string> ReadFileLines(const std::string& filename) {
  std::vector<std::string> lines;
  size_t dataSize = 0;
  void* data = SDL_LoadFile(filename.c_str(), &dataSize);
  if (data == nullptr) {
    Logger::Error("Failed to open file: " + filename + ": " + std::string(SDL_GetError()));
    return lines;
  }

  std::istringstream stream(std::string(static_cast<char*>(data), dataSize));
  SDL_free(data);

  std::string line;
  while (std::getline(stream, line)) {
    lines.push_back(line);
  }
  return lines;
}
}  // namespace

void LuaModuleBinding<IoModule>::install(sol::state& lua, Game& /*game*/) {
  lua.set_function("read_file_lines", &ReadFileLines);
}
