#pragma once

#include <optional>
#include <string>
#include <unordered_map>

#include "EngineOptions.h"
#include "General/Constants.h"
#include "General/Logger.h"

class GameConfig {
 public:
  GameConfig() = default;
  ~GameConfig() = default;

  GameConfig(const GameConfig&) = default;
  GameConfig& operator=(const GameConfig&) = default;

  GameConfig(GameConfig&&) = default;
  GameConfig& operator=(GameConfig&&) = default;

  bool LoadConfig(const std::unordered_map<std::string, std::string>& settings);

  bool LoadConfigFromFile(const std::string& assetPath);

  [[nodiscard]] const EngineOptions& GetEngineOptions() const;
  [[nodiscard]] EngineOptions& GetEngineOptions();

  [[nodiscard]] std::string GetAssetPath() const;
  [[nodiscard]] std::string GetGameTitle() const;
  [[nodiscard]] std::string GetStartupScript() const;
  [[nodiscard]] std::optional<std::string> GetDefaultScaleMode() const;
  [[nodiscard]] int GetDefaultWidth() const;
  [[nodiscard]] int GetDefaultHeight() const;

  int windowWidth{};
  int windowHeight{};
  float playableAreaWidth{};
  float playableAreaHeight{};

 private:
  template <typename T>
  bool SetValue(const std::unordered_map<std::string, std::string>& config, const std::string& key,
                void (GameConfig::*setter)(T), const std::function<T(const std::string&)>& converter,
                const bool required = false) {
    if (const auto it = config.find(key); it != config.end()) {
      try {
        T value = converter(it->second);
        (this->*setter)(value);
        return true;
      } catch (const std::exception& e) {
        Logger::Error("Error converting config value for key '" + key + "': " + e.what());
        return false;
      }
    }
    if (required) {
      Logger::Error("Required config setting not found: " + key);
      return false;
    }
    Logger::Info("Optional config setting not found: " + key);
    return true;
  }

  template <typename T>
  bool SetValue(const std::unordered_map<std::string, std::string>& config, const std::string& key,
                void (GameConfig::*setter)(T), const bool required = false) {
    static_assert(std::is_same_v<T, const std::string&> || std::is_same_v<T, std::string>,
                  "This SetValue overload is only for setters taking a std::string or const std::string&.");

    if (const auto it = config.find(key); it != config.end()) {
      (this->*setter)(it->second);
      return true;
    }
    if (required) {
      Logger::Error("Required config setting not found: " + key);
      return false;
    }
    Logger::Info("Optional config setting not found: " + key);
    return true;
  }

  bool SetValue(const std::unordered_map<std::string, std::string>& config, const std::string& key,
                void (GameConfig::*setter)(int), const bool required = false) {
    return SetValue<int>(config, key, setter, [](const std::string& s) { return std::stoi(s); }, required);
  }

  void SetAssetPath(const std::string& assetPath);
  void SetGameTitle(const std::string& gameTitle);
  void SetStartupScript(const std::string& startupScript);
  void SetDefaultScaleMode(const std::string& defaultScaleMode);
  void SetDefaultWidth(int defaultWidth);
  void SetDefaultHeight(int defaultHeight);

  bool has_loaded_config_ = false;

  EngineOptions engine_options_;
  std::string asset_path_;
  std::string game_title_;
  std::string startup_script_;
  std::optional<std::string> default_scaling_mode_;
  int default_width_ = Constants::kDefaultWindowWidth;
  int default_height_ = Constants::kDefaultWindowHeight;
};
