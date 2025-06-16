//
// Created by mblackman on 5/30/25.
//

#include "GameConfig.h"

#include <SDL3/SDL_storage.h>
#include <SDL3/SDL_timer.h>

#include <sstream>
#include <unordered_map>

#include "../General/Logger.h"

inline constexpr auto kConfigFileName = "config.ini";
inline constexpr auto kWhiteSpaceSymbols = " \t\n\r\f\v";

std::string TrimRight(const std::string &s) {
  const size_t end = s.find_last_not_of(kWhiteSpaceSymbols);
  return end == std::string::npos ? "" : s.substr(0, end + 1);
}

inline auto ReadConfigFile(void *dst, const Uint64 dstLen) -> std::unordered_map<std::string, std::string> {
  if (dst == nullptr || dstLen == 0) {
    Logger::Warn("Attempted to read an empty or null config file buffer.");
    return {};
  }

  auto config = std::unordered_map<std::string, std::string>();
  const auto content = std::string(static_cast<char *>(dst), dstLen);
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

bool GameConfig::LoadConfigFromFile(const std::string &assetPath) {
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

  if (!SDL_GetStorageFileSize(storage, kConfigFileName, &dstLen) && dstLen > 0) {
    Logger::Error(std::string(SDL_GetError()));
    return false;
  }

  void *dst = SDL_malloc(dstLen);
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

bool GameConfig::LoadConfig(const std::unordered_map<std::string, std::string> &settings) {
  bool success = true;

  success &= SetValue(settings, "Title", &GameConfig::SetGameTitle, true);
  success &= SetValue(settings, "StartupScript", &GameConfig::SetStartupScript, true);
  success &= SetValue(settings, "DefaultScalingMode", &GameConfig::SetDefaultScaleMode, false);
  success &= SetValue(settings, "DefaultWindowWidth", &GameConfig::SetDefaultWidth, false);
  success &= SetValue(settings, "DefaultWindowHeight", &GameConfig::SetDefaultHeight, false);

  return success;
}

const EngineOptions &GameConfig::GetEngineOptions() const { return engine_options_; }

EngineOptions &GameConfig::GetEngineOptions() { return engine_options_; }

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

void GameConfig::SetAssetPath(const std::string &assetPath) {
  if (assetPath.empty()) {
    Logger::Warn("Attempted to set empty asset path. Keeping current if any.");
    return;
  }
  asset_path_ = assetPath;
  Logger::Info("Asset path set to: " + asset_path_);
}

void GameConfig::SetGameTitle(const std::string &gameTitle) {
  if (gameTitle.empty()) {
    Logger::Warn("Attempted to set empty game title. Keeping current if any.");
    return;
  }
  game_title_ = gameTitle;
  Logger::Info("Game title set to: " + game_title_);
}

void GameConfig::SetStartupScript(const std::string &startupScript) {
  if (startupScript.empty()) {
    Logger::Warn("Attempted to set empty startup script. Keeping current if any.");
    return;
  }
  startup_script_ = startupScript;
  Logger::Info("Startup script set to: " + startup_script_);
}

void GameConfig::SetDefaultScaleMode(const std::string &defaultScaleMode) {
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
