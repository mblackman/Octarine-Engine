#pragma once

#include <sol/sol.hpp>
#include <string>
#include <vector>

class Registry;

// Owns the scene lifecycle Game used to inline: parse a scene script (table / function / side-
// effect forms), acquire-before-release of the scene's assets, load its entities, and track the
// acquired ids so the next swap or StopScene releases them. Reads the live SDL renderer + mixer
// off the registry's EngineContext, so it needs only the Registry + the shared sol::state.
//
// Game delegates its LuaBindingContext scene-op overrides (LoadScene/ReloadScene/StopScene/
// TrackSceneAssets) straight through to this; the `scene.*` Lua module binds against the same
// LuaBindingContext surface, so scripts reach these via Game.
class SceneLoader {
 public:
  SceneLoader(Registry* registry, sol::state& lua) : registry_(registry), lua_(lua) {}

  SceneLoader(const SceneLoader&) = delete;
  SceneLoader& operator=(const SceneLoader&) = delete;
  SceneLoader(SceneLoader&&) = delete;
  SceneLoader& operator=(SceneLoader&&) = delete;

  ~SceneLoader() = default;

  void LoadScene(const std::string& scenePath);
  void ReloadScene();
  void StopScene();

  // Record asset ids acquired for the current scene so StopScene/the next LoadScene releases
  // them. Deduped against ids already tracked. Called by the C++ scene loader and by the
  // `acquire_scene_assets` Lua global (which serves scenes that load via a side-effect script
  // rather than returning a table). Safe to call repeatedly with overlapping sets.
  void TrackSceneAssets(const std::vector<std::string>& assetIds);

  // True between a successful LoadScene/ReloadScene and the next StopScene. The editor toolbar
  // uses this to decide whether Play should resume a paused scene or (re)start a stopped one.
  [[nodiscard]] bool IsSceneRunning() const { return scene_running_; }

 private:
  // Clear the current scene's user entities and reset per-scene Lua input state, without
  // touching acquired assets. Asset release is sequenced separately so a scene swap can acquire
  // the next scene's set before releasing the previous one (acquire-before-release).
  void clearSceneEntities();

  Registry* registry_;
  sol::state& lua_;
  bool scene_running_ = false;
  // Asset ids acquired for the currently loaded scene. StopScene releases these; LoadScene
  // acquires the next scene's set first (acquire-before-release) so shared assets never churn.
  std::vector<std::string> current_scene_assets_;
};
