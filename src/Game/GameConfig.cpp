//
// Created by mblackman on 5/30/25.
//

#include "GameConfig.h"

#include <SDL3/SDL_filesystem.h>
#include <SDL3/SDL_storage.h>
#include <SDL3/SDL_timer.h>

#include <fstream>
#include <sstream>
#include <unordered_map>

#include "General/Logger.h"
#include "General/Utils.h"

inline constexpr auto kConfigFileName = "config.ini";
inline constexpr auto kPreferencesFileName = "preferences.ini";
inline constexpr auto kWhiteSpaceSymbols = " \t\n\r\f\v";

std::string TrimRight(const std::string& s) {
  const size_t end = s.find_last_not_of(kWhiteSpaceSymbols);
  return end == std::string::npos ? "" : s.substr(0, end + 1);
}

inline auto ReadConfigFile(void* dst, const Uint64 dstLen) -> std::unordered_map<std::string, std::string> {
  if (dst == nullptr || dstLen == 0) {
    Logger::Warn("Attempted to read an empty or null config file buffer.");
    return {};
  }

  auto config = std::unordered_map<std::string, std::string>();
  const auto content = std::string(static_cast<char*>(dst), dstLen);
  if (content.empty()) {
    Logger::Warn("Config file content is empty after conversion.");
    return {};
  }

  auto stream = std::istringstream(content);
  std::string line;

  while (getline(stream, line, '\n')) {
    const size_t first = line.find_first_not_of(kWhiteSpaceSymbols);
    if (std::string::npos == first) {
      continue;
    }

    line = line.substr(first);
    line = TrimRight(line);

    if (line.empty() || line[0] == '[' || line[0] == '#') {
      continue;
    }

    const auto keyValue = line.find('=');
    if (keyValue == std::string::npos) {
      Logger::Warn("Could not find config settings on line: " + line);
      continue;
    }

    auto key = line.substr(0, keyValue);
    config[key] = line.substr(keyValue + 1);
  }

  return config;
}

bool GameConfig::LoadConfigFromFile(const std::string& assetPath) {
  const auto storage = SDL_OpenTitleStorage(assetPath.c_str(), 0);

  if (storage == nullptr) {
    Logger::Error("Failed to open title storage: " + assetPath);
    Logger::Error(std::string(SDL_GetError()));
    return false;
  }

  while (!SDL_StorageReady(storage)) {
    SDL_Delay(1);
  }

  Uint64 dstLen = 0;

  if (!SDL_GetStorageFileSize(storage, kConfigFileName, &dstLen)) {
    Logger::Error("Failed to size config file: " + std::string(SDL_GetError()));
    return false;
  }

  if (dstLen == 0) {
    Logger::Error("Config file is empty: " + std::string(kConfigFileName));
    return false;
  }

  void* dst = SDL_malloc(dstLen);
  if (!dst) {
    Logger::Error("SDL_malloc failed for config buffer: " + std::string(SDL_GetError()));
    return false;
  }

  if (!SDL_ReadStorageFile(storage, kConfigFileName, dst, dstLen)) {
    Logger::Error(std::string(SDL_GetError()));
    SDL_free(dst);
    return false;
  }

  const auto config = ReadConfigFile(dst, dstLen);
  SDL_free(dst);

  if (config.empty()) {
    Logger::Error("Failed to read config file: " + std::string(kConfigFileName));
    return false;
  }

  // Create the instance and populate it with the config settings
  SetAssetPath(assetPath);
  has_loaded_config_ = true;
  return LoadConfig(config);
}

void GameConfig::SaveUserPreferences() {
  if (!has_loaded_config_) return;

  std::ofstream file(asset_path_ + "/" + kPreferencesFileName);
  if (!file.is_open()) {
    Logger::Error("Failed to open preferences file for writing: " + asset_path_ + "/" + kPreferencesFileName);
    return;
  }

  file << "showDebugGUI=" << (engine_options_.showDebugGUI ? "true" : "false") << "\n";
  file << "drawColliders=" << (engine_options_.drawColliders ? "true" : "false") << "\n";
  file << "showFpsCounter=" << (engine_options_.showFpsCounter ? "true" : "false") << "\n";
  file << "showEntityInfo=" << (engine_options_.showEntityInfo ? "true" : "false") << "\n";
  // showPerfOverlay is intentionally NOT persisted here: it is a project config.ini knob
  // (PerfOverlay=) and config.ini is its single source of truth. Persisting it to preferences (which
  // loads after config.ini) would let a stale prefs value silently shadow the config the dev edits.
  // The editor's "Show Perf Overlay" checkbox is therefore a live, session-only toggle.
  file << "masterVolume=" << engine_options_.masterVolume << "\n";

  file.close();
}

void GameConfig::LoadUserPreferences() {
  if (!has_loaded_config_) return;

  std::ifstream file(asset_path_ + "/" + kPreferencesFileName);
  if (!file.is_open()) {
    return;  // Silent because it's okay if preferences don't exist yet
  }

  std::string line;
  while (std::getline(file, line)) {
    const auto keyValue = line.find('=');
    if (keyValue == std::string::npos) continue;

    const auto key = line.substr(0, keyValue);
    const auto value = line.substr(keyValue + 1);

    if (key == "showDebugGUI")
      engine_options_.showDebugGUI = (value == "true");
    else if (key == "drawColliders")
      engine_options_.drawColliders = (value == "true");
    else if (key == "showFpsCounter")
      engine_options_.showFpsCounter = (value == "true");
    else if (key == "showEntityInfo")
      engine_options_.showEntityInfo = (value == "true");
    // showPerfOverlay deliberately not read here — config.ini's PerfOverlay= is authoritative, so a
    // legacy line left in preferences.ini (from before this knob was config-only) is ignored rather
    // than shadowing config.
    else if (key == "masterVolume")
      engine_options_.masterVolume = std::stof(value);
  }
}

bool GameConfig::LoadConfig(const std::unordered_map<std::string, std::string>& settings) {
  bool success = true;

  // LogLevel is parsed first so subsequent config-loading Logger::Info calls already honor it.
  success &= SetValue(settings, "LogLevel", &GameConfig::SetLogLevel, false);
  success &= SetValue(settings, "Title", &GameConfig::SetGameTitle, true);
  success &= SetValue(settings, "StartupScript", &GameConfig::SetStartupScript, true);
  success &= SetValue(settings, "DefaultScalingMode", &GameConfig::SetDefaultScaleMode, false);
  success &= SetValue(settings, "DefaultWindowWidth", &GameConfig::SetDefaultWidth, false);
  success &= SetValue(settings, "DefaultWindowHeight", &GameConfig::SetDefaultHeight, false);
  success &= SetValue(settings, "FpsTarget", &GameConfig::SetFpsTarget, false);
  success &= SetValue(settings, "HotReload", &GameConfig::SetHotReloadEnabled, false);
  success &= SetValue(settings, "HotReloadPollSeconds", &GameConfig::SetHotReloadPollSeconds, false);
  success &= SetValue(settings, "PerfOverlay", &GameConfig::SetPerfOverlay, false);
  success &= SetValue(settings, "PerfOverlayCorner", &GameConfig::SetPerfOverlayCorner, false);
  success &= SetValue(settings, "PerfOverlayMetrics", &GameConfig::SetPerfOverlayMetrics, false);

  return success;
}

const EngineOptions& GameConfig::GetEngineOptions() const { return engine_options_; }

EngineOptions& GameConfig::GetEngineOptions() { return engine_options_; }

std::string GameConfig::GetAssetPath() const {
  assert(has_loaded_config_);
  return asset_path_;
}

std::string GameConfig::GetGameTitle() const {
  assert(has_loaded_config_);
  return game_title_;
}

std::string GameConfig::GetStartupScript() const {
  assert(has_loaded_config_);
  return startup_script_;
}

std::optional<std::string> GameConfig::GetDefaultScaleMode() const {
  assert(has_loaded_config_);
  return default_scaling_mode_;
}

int GameConfig::GetDefaultWidth() const {
  assert(has_loaded_config_);
  return default_width_;
}

int GameConfig::GetDefaultHeight() const {
  assert(has_loaded_config_);
  return default_height_;
}

void GameConfig::SetAssetPath(const std::string& assetPath) {
  if (assetPath.empty()) {
    Logger::Warn("Attempted to set empty asset path. Keeping current if any.");
    return;
  }
  asset_path_ = assetPath;
  Logger::Info("Asset path set to: " + asset_path_);
}

void GameConfig::SetGameTitle(const std::string& gameTitle) {
  if (gameTitle.empty()) {
    Logger::Warn("Attempted to set empty game title. Keeping current if any.");
    return;
  }
  game_title_ = gameTitle;
  Logger::Info("Game title set to: " + game_title_);
}

void GameConfig::SetStartupScript(const std::string& startupScript) {
  if (startupScript.empty()) {
    Logger::Warn("Attempted to set empty startup script. Keeping current if any.");
    return;
  }
  startup_script_ = startupScript;
  Logger::Info("Startup script set to: " + startup_script_);
}

void GameConfig::SetDefaultScaleMode(const std::string& defaultScaleMode) {
  if (defaultScaleMode.empty()) {
    default_scaling_mode_ = std::nullopt;
    Logger::Info("Default scaling mode set to not present (empty string provided).");
  } else {
    default_scaling_mode_ = defaultScaleMode;
    Logger::Info("Default scaling mode set to: " + *default_scaling_mode_);
  }
}

void GameConfig::SetDefaultWidth(const int defaultWidth) { default_width_ = defaultWidth; }
void GameConfig::SetDefaultHeight(const int defaultHeight) { default_height_ = defaultHeight; }

void GameConfig::SetFpsTarget(const int fpsTarget) {
  if (fpsTarget < 0) {
    Logger::Warn("FpsTarget must be >= 0 (0 = uncapped); keeping current value.");
    return;
  }
  engine_options_.fpsTarget = fpsTarget;
  Logger::Info(fpsTarget == 0 ? std::string("FPS target: uncapped") : "FPS target: " + std::to_string(fpsTarget));
}

void GameConfig::SetHotReloadEnabled(const bool enabled) {
  engine_options_.hotReloadEnabled = enabled;
  Logger::Info(std::string("Hot reload ") + (enabled ? "enabled" : "disabled"));
}

void GameConfig::SetPerfOverlay(const bool enabled) {
  engine_options_.showPerfOverlay = enabled;
  Logger::Info(std::string("Perf overlay ") + (enabled ? "enabled" : "disabled"));
}

void GameConfig::SetPerfOverlayCorner(const std::string& corner) {
  if (corner == "top-left") {
    engine_options_.perfOverlayCorner = PerfOverlayCorner::TopLeft;
  } else if (corner == "top-right") {
    engine_options_.perfOverlayCorner = PerfOverlayCorner::TopRight;
  } else if (corner == "bottom-left") {
    engine_options_.perfOverlayCorner = PerfOverlayCorner::BottomLeft;
  } else if (corner == "bottom-right") {
    engine_options_.perfOverlayCorner = PerfOverlayCorner::BottomRight;
  } else {
    Logger::Warn("Unknown PerfOverlayCorner '" + corner +
                 "' (expected top-left|top-right|bottom-left|bottom-right); keeping current.");
    return;
  }
  Logger::Info("Perf overlay corner: " + corner);
}

void GameConfig::SetPerfOverlayMetrics(const std::string& metrics) {
  const auto parts = SplitString(metrics, ',');
  PerfOverlayMetrics perfOverlayMetrics = {};

  for (const auto& metric : parts) {
    if (metric == "fps") {
      perfOverlayMetrics = perfOverlayMetrics | PerfOverlayMetrics::Fps;
    } else if (metric == "frametime" || metric == "frame-time" || metric == "ms") {
      perfOverlayMetrics = perfOverlayMetrics | PerfOverlayMetrics::FrameTime;
    } else if (metric == "entities") {
      perfOverlayMetrics = perfOverlayMetrics | PerfOverlayMetrics::Entities;
    } else if (metric == "memory" || metric == "mem") {
      perfOverlayMetrics = perfOverlayMetrics | PerfOverlayMetrics::Memory;
    } else if (metric == "all" || metric == "both") {
      perfOverlayMetrics = PerfOverlayMetrics::All;
    } else {
      Logger::Warn("Unknown PerfOverlayMetrics '" + metric +
                   "' (expected fps|frametime|entities|memory|all); keeping current.");
      return;
    }
  }

  engine_options_.perfOverlayMetrics = perfOverlayMetrics;
  Logger::Info("Perf overlay metrics: " + metrics);
}

void GameConfig::SetHotReloadPollSeconds(const float seconds) {
  if (seconds <= 0.0F) {
    Logger::Warn("HotReloadPollSeconds must be > 0; keeping current value.");
    return;
  }
  engine_options_.hotReloadPollSeconds = seconds;
}

void GameConfig::SetLogLevel(const std::string& logLevel) {
  if (logLevel.empty()) return;
  Logger::SetLevel(logLevel);
  Logger::Info("Log level set to: " + logLevel);
}
