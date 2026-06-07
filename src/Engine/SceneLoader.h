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

  // Request a scene swap. Once the frame loop is running (its first FlushPendingSceneLoad arms
  // deferral), this only records the path; the actual swap runs at the next FlushPendingSceneLoad()
  // — i.e. the top of FrameLoop::Update, outside any event dispatch / system ForEach. That deferral
  // is what makes it safe for a UIButton on_click handler to navigate: doing the swap inline would
  // clear + recreate entities in the same archetype chunk UIButtonSystem is mid-iteration over,
  // re-firing the click on a freshly created (and not-yet-transformed, so positioned at the origin)
  // button. Before deferral is armed (bake + the initial startup load) the swap happens immediately.
  void LoadScene(const std::string& scenePath);
  void ReloadScene();
  void StopScene();

  // Arm deferral (first call) and perform a pending deferred swap, if any. Called at the top of the
  // frame, before systems run, so the new scene's entities get their GlobalTransformComponent
  // populated this same frame. Never reached under bake (no frame loop), so bake loads stay immediate.
  void FlushPendingSceneLoad();

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
  // Deferred-swap state. deferred_swaps_ flips on once the live loop is running; while set, LoadScene
  // only stashes pending_scene_path_ (last request wins) and FlushPendingSceneLoad does the work.
  bool deferred_swaps_ = false;
  bool has_pending_scene_ = false;
  std::string pending_scene_path_;
  // Asset ids acquired for the currently loaded scene. StopScene releases these; LoadScene
  // acquires the next scene's set first (acquire-before-release) so shared assets never churn.
  std::vector<std::string> current_scene_assets_;
};
