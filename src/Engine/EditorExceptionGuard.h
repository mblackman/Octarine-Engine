#pragma once

#include <exception>
#include <string>

#include "Game/GameConfig.h"
#include "General/Logger.h"

namespace octarine::editor {

// Run a piece of game-driving work (`fn`) that may execute project/Lua code and could throw.
//
// In an editor session a project bug must not take down the whole editor process. When `fn` throws
// there, the exception is caught, logged (so it surfaces in the editor console), and the running
// game is paused — leaving the editor alive so the author can read the error, fix the script, and
// hit Play again. Pausing on catch also stops a throwing per-frame tick from re-raising the same
// exception every frame.
//
// Outside an editor session (the standalone player, headless bench/bake tooling) the exception is
// left to propagate exactly as before: the player runs in its own process, so a crash there never
// reaches the editor, and we don't want to silently swallow failures in shipped/CI paths.
//
// `context` is a short human label for the failing phase (e.g. "Update", "Input", "ReloadScene")
// included in the log line. Returns true when `fn` completed without throwing, false when it was
// caught.
template <typename Fn>
bool RunEditorGuarded(GameConfig& config, const char* context, Fn&& fn) {
  if (!config.IsEditorMode()) {
    fn();
    return true;
  }

  try {
    fn();
    return true;
  } catch (const std::exception& ex) {
    Logger::Error(std::string("[editor] Game exception in ") + context + ": " + ex.what() + " — execution paused.");
  } catch (...) {
    Logger::Error(std::string("[editor] Unknown game exception in ") + context + " — execution paused.");
  }
  config.GetEngineOptions().isPaused = true;
  return false;
}

}  // namespace octarine::editor
