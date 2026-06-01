#pragma once

#include <spdlog/logger.h>

#include <memory>
#include <mutex>
#include <string>
#include <vector>

class Logger {
  static std::shared_ptr<spdlog::logger> lua_logger_;
  static std::vector<std::string> history_;
  static std::mutex history_mutex_;

 public:
  static void Init();
  // Apply a runtime log level override (trace|debug|info|warn|error|critical|off). Case-insensitive;
  // unknown values log a warning and leave the current level intact. Typically called once from
  // Game::Setup after config.ini's `LogLevel=` is parsed, on top of the compile-time default.
  static void SetLevel(const std::string& level);
  static void Error(const std::string& message);
  static void Warn(const std::string& message);
  static void Info(const std::string& message);
  static void LogLua(const std::string& message);
  static void ErrorLua(const std::string& message);
  static void WarnLua(const std::string& message);
  static void InfoLua(const std::string& message);

  static std::vector<std::string> GetHistory();
  static void ForEachHistory(const std::function<void(const std::string&)>& callback);
  static void ClearHistory();
};