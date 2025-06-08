//
// Created by mblackman on 5/30/25.
//

#pragma once

#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <unordered_map>

class GameConfig {
public:
  GameConfig() = default;

 GameConfig(const GameConfig&) = delete;
 GameConfig& operator=(const GameConfig&) = delete;

 GameConfig(GameConfig&&) = delete;
 GameConfig& operator=(GameConfig&&) = delete;

 ~GameConfig() = default;

  bool LoadConfig(const std::unordered_map<std::string, std::string>& settings);

  bool LoadConfigFromFile(const std::string& assetPath);

  [[nodiscard]] std::string GetAssetPath() const;
  [[nodiscard]] std::string GetGameTitle() const;
  [[nodiscard]] std::string GetStartupScript() const;
  [[nodiscard]] std::optional<std::string> GetDefaultScaleMode() const;

private:
  void SetAssetPath(const std::string& assetPath);
  void SetGameTitle(const std::string& gameTitle);
  void SetStartupScript(const std::string& startupScript);
  void SetDefaultScaleMode(const std::string& defaultScaleMode);

  static std::unique_ptr<GameConfig> instance_;
  static std::once_flag once_flag_;

  std::string asset_path_;
  std::string game_title_;
  std::string startup_script_;
  std::optional<std::string> default_scaling_mode_;
};