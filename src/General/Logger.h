#pragma once

#include <spdlog/logger.h>

#include <chrono>
#include <cstdint>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

class Logger {
 public:
  // Captured Lua script error for editor toast notifications.
  struct ScriptError {
    std::uint64_t sequence{};
    std::string message;
    std::chrono::steady_clock::time_point when{};
  };

 private:
  static std::shared_ptr<spdlog::logger> lua_logger_;
  static std::vector<std::string> history_;
  static std::mutex history_mutex_;
  static std::vector<ScriptError> script_errors_;
  static std::uint64_t script_error_sequence_;

  // Appends entry and enforces bounded capacity under history_mutex_.
  static void PushHistory(std::string entry);

 public:
  static void Init();
  // Sets runtime log level (trace|debug|info|warn|error|critical|off).
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

  // Returns snapshot of recent script errors for editor UI.
  static std::vector<ScriptError> RecentScriptErrors();
};