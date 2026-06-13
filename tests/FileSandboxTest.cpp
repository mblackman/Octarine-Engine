// Unit checks for the sandboxed Lua file-write surface. gtest-free; exit code = failed-check
// count. Covers the file_sandbox helpers (lexical containment + atomic write) directly, then the
// storage.* / project.* modules end-to-end through a fake LuaBindingContext host — including the
// editor-session gate that keeps project.* out of player sessions.

#include <filesystem>
#include <fstream>
#include <sol/sol.hpp>
#include <sstream>
#include <string>
#include <vector>

#include "Engine/EngineContext.h"
#include "General/FileSandbox.h"
#include "General/Logger.h"
#include "Lua/LuaBindingContext.h"
#include "Lua/Modules/ProjectModuleLuaBinding.h"
#include "Lua/Modules/StorageModuleLuaBinding.h"
#include "TestHarness.h"

using octarine::test::Check;

namespace {

class FakeHost : public LuaBindingContext {
 public:
  FakeHost(std::string saveRoot, std::string projectRoot, const bool editorMode)
      : save_root_(std::move(saveRoot)), project_root_(std::move(projectRoot)), editor_mode_(editorMode) {}

  [[nodiscard]] Registry* GetRegistry() const override { return nullptr; }
  [[nodiscard]] SDL_Renderer* GetRenderer() const override { return nullptr; }
  [[nodiscard]] EngineContext& GetContext() override { return context_; }

  void LoadScene(const std::string& /*scenePath*/) override {}
  void ReloadScene() override {}
  void StopScene() override {}
  void TrackSceneAssets(const std::vector<std::string>& /*assetIds*/) override {}

  [[nodiscard]] bool IsBakeMode() const override { return false; }
  void RecordBakeValidationFailures(int /*failures*/) override {}

  [[nodiscard]] bool IsEditorMode() const override { return editor_mode_; }
  [[nodiscard]] std::string GetProjectPath() const override { return project_root_; }
  std::string GetSaveDataPath() override { return save_root_; }

  void RequestQuit() override {}

 private:
  EngineContext context_{};
  std::string save_root_;
  std::string project_root_;
  bool editor_mode_;
};

std::string ReadAll(const std::filesystem::path& path) {
  std::ifstream file(path, std::ios::binary);
  std::ostringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}

void TestResolve(const std::string& root) {
  const auto base = std::filesystem::path(root).lexically_normal();
  Check(file_sandbox::Resolve(root, "a/b.txt") == (base / "a/b.txt").lexically_normal().string(),
        "Resolve joins a plain relative path");
  Check(!file_sandbox::Resolve(root, "a/../b.txt").empty(), "Resolve allows ../ that stays inside the root");
  Check(file_sandbox::Resolve(root, "../escape.txt").empty(), "Resolve rejects ../ escape");
  Check(file_sandbox::Resolve(root, "a/../../escape.txt").empty(), "Resolve rejects nested ../ escape");
  Check(file_sandbox::Resolve(root, (base / "abs.txt").string()).empty(), "Resolve rejects absolute paths");
  Check(file_sandbox::Resolve("", "a.txt").empty(), "Resolve rejects an empty root");
}

void TestWriteFileAtomic(const std::filesystem::path& root) {
  const auto target = root / "atomic" / "out.txt";
  Check(file_sandbox::WriteFileAtomic(target.string(), "first"), "WriteFileAtomic creates parent dirs + file");
  Check(ReadAll(target) == "first", "WriteFileAtomic wrote the contents");
  Check(!std::filesystem::exists(target.string() + ".tmp"), "WriteFileAtomic leaves no .tmp behind");
  Check(file_sandbox::WriteFileAtomic(target.string(), "second"), "WriteFileAtomic overwrites an existing file");
  Check(ReadAll(target) == "second", "WriteFileAtomic replaced the contents");
}

void TestStorageModule(LuaBindingContext& host, const std::filesystem::path& saveRoot) {
  sol::state lua;
  lua.open_libraries(sol::lib::base, sol::lib::string, sol::lib::table);
  LuaModuleBinding<StorageModule>::install(lua, host);

  Check(lua["storage"].get_type() == sol::type::table, "storage table installed");
  Check(lua.script("return storage.write('saves/slot1.json', 'hello')").get<bool>(), "storage.write");
  Check(std::filesystem::exists(saveRoot / "saves/slot1.json"), "storage.write landed under the save root");
  Check(lua.script("return storage.read('saves/slot1.json')").get<std::string>() == "hello", "storage.read roundtrip");
  Check(lua.script("return storage.read('saves/missing.json') == nil").get<bool>(),
        "storage.read returns nil for a missing file");
  Check(lua.script("return storage.exists('saves/slot1.json')").get<bool>(), "storage.exists");
  Check(lua.script("return storage.list('saves')[1]").get<std::string>() == "slot1.json", "storage.list");
  Check(lua.script("return storage.list()[1]").get<std::string>() == "saves", "storage.list() lists the save root");
  Check(!lua.script("return storage.write('../evil.txt', 'x')").get<bool>(), "storage.write rejects traversal");
  Check(!std::filesystem::exists(saveRoot.parent_path() / "evil.txt"), "traversal write created nothing");
  Check(lua.script("return storage.remove('saves/slot1.json')").get<bool>(), "storage.remove");
  Check(!std::filesystem::exists(saveRoot / "saves/slot1.json"), "storage.remove deleted the file");
}

void TestTableRoundtrip(LuaBindingContext& host) {
  sol::state lua;
  lua.open_libraries(sol::lib::base, sol::lib::string, sol::lib::table, sol::lib::math);
  LuaModuleBinding<StorageModule>::install(lua, host);

  Check(lua.script(R"(
    local state = {
      level = 3, hp = 72.5, name = 'Rince"wind\n', alive = true,
      inventory = { 'sword', 'luggage' },
      flags = { [false] = 'no', [true] = 'yes' },
      big = 2^53, neg = -7, huge = math.huge,
    }
    return storage.write_table('saves/state.lua', state)
  )")
            .get<bool>(),
        "storage.write_table");

  Check(lua.script(R"(
    local s = storage.read_table('saves/state.lua')
    return s ~= nil
      and s.level == 3 and s.hp == 72.5 and s.name == 'Rince"wind\n' and s.alive == true
      and s.inventory[1] == 'sword' and s.inventory[2] == 'luggage'
      and s.flags[false] == 'no' and s.flags[true] == 'yes'
      and s.big == 2^53 and s.neg == -7 and s.huge == math.huge
  )")
            .get<bool>(),
        "storage.read_table roundtrips every value");

  Check(lua.script("return storage.read_table('saves/nope.lua') == nil").get<bool>(),
        "storage.read_table returns nil for a missing file");

  // A save file is data, not code: a tampered file that calls anything fails to load (its
  // environment is empty), and one that returns a non-table is rejected.
  Check(lua.script(R"(
    storage.write('saves/evil.lua', 'print("pwned") return {}')
    return storage.read_table('saves/evil.lua') == nil
  )")
            .get<bool>(),
        "storage.read_table runs tampered files without any globals");
  Check(lua.script(R"(
    storage.write('saves/notatable.lua', 'return 42')
    return storage.read_table('saves/notatable.lua') == nil
  )")
            .get<bool>(),
        "storage.read_table rejects a non-table result");

  // Unserializable inputs fail loudly instead of writing a partial save.
  Check(!lua.script("return storage.write_table('saves/fn.lua', { f = function() end })").get<bool>(),
        "storage.write_table rejects function values");
  Check(!lua.script(R"(
    local t = {}
    t.self = t
    return storage.write_table('saves/cycle.lua', t)
  )")
             .get<bool>(),
        "storage.write_table rejects cyclic tables");

  // Determinism: serializing the same logical table twice yields identical bytes.
  Check(lua.script(R"(
    storage.write_table('saves/a.lua', { z = 1, a = 2, [3] = 'c', [1] = 'a' })
    storage.write_table('saves/b.lua', { [1] = 'a', a = 2, [3] = 'c', z = 1 })
    return storage.read('saves/a.lua') == storage.read('saves/b.lua')
  )")
            .get<bool>(),
        "storage.write_table output is deterministic");
}

void TestProjectModule(const std::string& saveRoot, const std::filesystem::path& projectRoot) {
  {
    sol::state lua;
    lua.open_libraries(sol::lib::base);
    FakeHost player(saveRoot, projectRoot.string(), /*editorMode=*/false);
    LuaModuleBinding<ProjectModule>::install(lua, player);
    Check(!lua["project"].valid(), "project table absent outside an editor session");
  }

  sol::state lua;
  lua.open_libraries(sol::lib::base);
  FakeHost editor(saveRoot, projectRoot.string(), /*editorMode=*/true);
  LuaModuleBinding<ProjectModule>::install(lua, editor);
  Check(lua["project"].get_type() == sol::type::table, "project table installed in an editor session");
  Check(lua.script("return project.path()").get<std::string>() == projectRoot.string(), "project.path");
  Check(lua.script("return project.write_file('assets/generated/map.lua', 'return {}')").get<bool>(),
        "project.write_file");
  Check(ReadAll(projectRoot / "assets/generated/map.lua") == "return {}", "project.write_file wrote the contents");
  Check(!lua.script("return project.write_file('../evil.lua', 'x')").get<bool>(),
        "project.write_file rejects traversal");
}

}  // namespace

int main() {
  Logger::Init();

  const auto testRoot = std::filesystem::temp_directory_path() / "octarine-file-sandbox-test";
  std::filesystem::remove_all(testRoot);
  const auto saveRoot = testRoot / "save";
  const auto projectRoot = testRoot / "project";
  std::filesystem::create_directories(saveRoot);
  std::filesystem::create_directories(projectRoot);

  std::cout << "[file_sandbox helpers]\n";
  TestResolve(saveRoot.string());
  // Outside saveRoot so the storage.list() check below sees only what storage.write created.
  TestWriteFileAtomic(testRoot / "helper");

  std::cout << "[storage module]\n";
  FakeHost host(saveRoot.string(), projectRoot.string(), /*editorMode=*/false);
  TestStorageModule(host, saveRoot);

  std::cout << "[table serialization]\n";
  TestTableRoundtrip(host);

  std::cout << "[project module]\n";
  TestProjectModule(saveRoot.string(), projectRoot);

  std::filesystem::remove_all(testRoot);
  return octarine::test::Result();
}
